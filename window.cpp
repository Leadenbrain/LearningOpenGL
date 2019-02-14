#include <GL/glew.h>
#include <stdlib.h>
//#include <stdio.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "VertexBuffer.h"
#include "renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "shader.h"
#include "Texture.h"




int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if ( glewInit() != GLEW_OK)
    	std::cout << "GLEW_INIT != GLEW_OK" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    // Put this in a new scope so closing windows ends application
	{
	    float vert_pos[] = {
	    	-0.5f, -0.5f, 0.0f, 0.0f,	// index 0; texture positions
	    	 0.5f, -0.5f, 1.0f, 0.0f,	// index 1
	    	 0.5f,  0.5f, 1.0f, 1.0f,	// index 2
	    	-0.5f,  0.5f, 0.0f, 1.0f	// index 3
	    };

	    unsigned int indices[] = {
	    	0, 1, 2,
	    	2, 3, 0
	    };

	    GLCall(glEnable(GL_BLEND));
	    GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));


	    // 4 coords, 4 vertices, each floats
	    VertexArray va;
	    VertexBuffer vb(vert_pos, 4*4*sizeof(float));

	    // push elements to VB layout; vertex positions, texture co-ords
	    VertexBufferLayout layout;
	    layout.Push<float>(2);
	    layout.Push<float>(2);
	    va.AddBuffer(vb, layout);

	    IndexBuffer ib(indices, 6);

	    Shader shader("res/shaders/Basic.shader");
	    shader.Bind();
	    shader.SetUniform4f("u_Colour", 0.2f, 0.3f, 0.8f, 1.0f);

	    Texture texture("res/textures/TEST2.png");
	    texture.Bind();
	    shader.SetUniform1i("u_Texture",0);

	    va.Unbind();
	    vb.Unbind();
	    ib.Unbind();
	    shader.Unbind();

	    Renderer renderer;

	    float r = 0.0f;
	    float increment = 0.05f;

	    //glBindBuffer(GL_ARRAY_BUFFER, 0);

	    /* Loop until the user closes the window */
	    while (!glfwWindowShouldClose(window))
	    {
	        /* Render here */
	    	renderer.Clear();

		    shader.Bind();
		    shader.SetUniform4f("u_Colour", r, r*r , 1-r, 1.0f);
	        
	        renderer.Draw(va, ib, shader);

	        if (r > 1.0f)
	        	increment = -0.003f;
	        else if (r < 0.0f)
	        	increment =  0.003f;

	        r += increment;

	        /* Swap front and back buffers */
	        glfwSwapBuffers(window);

	        glfwSwapInterval(1);	// lock to monitor framerate

	        /* Poll for and process events */
	        glfwPollEvents();
	    }

	}
    glfwTerminate();
    return 0;
}