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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
			-50.0f, -50.0f, 0.0f, 0.0f,	// index 0; texture positions
			 50.0f, -50.0f, 1.0f, 0.0f,	// index 1
			 50.0f,  50.0f, 1.0f, 1.0f,	// index 2
			-50.0f,  50.0f, 0.0f, 1.0f	// index 3
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
		// 4:3 ratio (640x480)
		// perspective for 3D, this is jsut 2D
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Colour", 0.2f, 0.3f, 0.8f, 1.0f);
		// could be set every frame like colour, but not moving for now


		Texture texture("res/textures/TEST2.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture",0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		// initialize our GUI
		ImGui::CreateContext();
//	    ImGuiIO& io = ImGui::GetIO(); (void)io;
		const char* glsl_version = "#version 150";
		ImGui_ImplOpenGL3_Init(glsl_version);

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 400, 0);

		float r = 0.0f;
		float increment = 0.05f;

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			// put before any IMGUI code
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			shader.Bind();
			
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj* view * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			// Don't need to rebind shader because it is already bound from before
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj* view * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			if (r > 1.0f)
				increment = -0.003f;
			else if (r < 0.0f)
				increment =  0.003f;

			r += increment;

			{
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, (960.0f,540.0f));            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, (960.0f,540.0f));            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			glfwSwapInterval(1);	// lock to monitor framerate

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}