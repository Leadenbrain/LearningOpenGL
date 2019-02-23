#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform vec4 u_Colour;
uniform sampler2D u_Texture;
uniform float u_Time;

float spheres(vec3 z)
{
  z = mod((z),1.0)-vec3(0.5); // instance on xy-plane
  return length(z)-0.1;             // sphere DE
};

float Scale = 2.0;
int Iterations = 50;
float Power = 8.0;
float Bailout = 1.15;

float DE(vec3 z)
{
	vec3 a1 = vec3(1,1,1);
	vec3 a2 = vec3(-1,-1,1);
	vec3 a3 = vec3(1,-1,-1);
	vec3 a4 = vec3(-1,1,-1);
	vec3 c;
	int n = 0;
	float dist, d;
	while (n < Iterations) {
		 c = a1; dist = length(z-a1);
	        d = length(z-a2); if (d < dist) { c = a2; dist=d; }
		 d = length(z-a3); if (d < dist) { c = a3; dist=d; }
		 d = length(z-a4); if (d < dist) { c = a4; dist=d; }
		z = Scale*z-c*(Scale-1.0);
		n++;
	}

	return length(z) * pow(Scale, float(-n));
}

float DE3(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < Iterations ; i++) {
		r = length(z);
		if (r>Bailout) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}

float trace(vec3 o, vec3 r)
{
 	float t = 0.0;
        for (int i=0; i<100; ++i) {
            vec3 p = o + r*t;
            float d = spheres(p);
            t += d*0.5;
        }
    return t;
};

void main()
{
	vec2 uv = 2*v_TexCoord - 1.0;
    vec3 r = normalize(vec3(uv,1.0));
    vec3 o = vec3(-0.0+u_Time*0.001,0.0,-2.0+u_Time*0.0004);

    float t = trace(o,r);

    float fog = 1.0/(1.0+t*t*t*0.009);

    vec3 fc = vec3(fog,0.5*fog, 0.2*fog);
    vec4 fragColour = vec4(fc,1.0);
	vec4 texColour = texture(u_Texture, v_TexCoord);
	colour = texColour;
	colour = fragColour;
};