
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

float resx = 900;
float resy = 500;

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
	window = glfwCreateWindow(resx, resy, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // enables vsync
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW ERROR!!!" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	{
        

		float vertices[] = {
			-50, -50, 0.f, 0.0f, 0.0f,
			 50, -50, 0.f, 1.0f, 0.0f,
			 50,  50, 0.f, 1.0f, 1.0f,
			-50,  50, 0.f, 0.0f, 1.0f,
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_DEPTH_TEST));
        //GLCall(glDepthMask(GL_FALSE));
        GLCall(glDepthFunc(GL_LESS));
		VertexArray va;
		// 4 vertices, 4 floats data per vertex
		VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(2);
		va.addBuffer(vb, layout);
		IndexBuffer ib(indices, 6);


		Shader shader("res/shaders/generic.shader");
		shader.bind();
		//shader.setUniform4f("u_Color", 0.2, 0.3, 0.8, 1.0);

		Texture texture("res/textures/bad_president.png");
        Texture textureSkull("res/textures/boid.png");
		texture.bind();
		shader.setUniform1i("u_Texture", 0);

        Model mdl1(ib, va, shader);
        Model mdl2(ib, va, shader);
        Mesh mSkull("res/models/boid.obj");
        Model mdlSkull(*mSkull.ib, mSkull.va, shader, &textureSkull);

		// unbind everything
		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		Renderer renderer;
        //glm::mat4 proj = glm::ortho(0.f, resx, 0.f, resy, -1.f, 1.f);
        renderer.proj = glm::perspective(glm::radians(100.f), float(resx) / float(resy), 0.1f, 10000.f);

        //renderer.view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -resx * .5));
        renderer.view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -resx * .5));


		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		//const char* glsl_version = "#version 330 core";
		//ImGui_ImplOpenGL3_Init(glsl_version);


        float val = 0.0;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
            val += 0.01;
			/* Poll for and process events */
			glfwPollEvents();
			
			/* Render here */
			//GLCall(glClear(GL_COLOR_BUFFER_BIT));
			renderer.clear();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			static glm::vec3 translationA = glm::vec3(0, 0, 0);
			static glm::vec3 translationB = glm::vec3(400, 200, 0);
            static glm::vec3 rotationA    = glm::vec3(0, 0, -1);
            static glm::vec3 rotationB    = glm::vec3(0, 0, 0);
            static glm::vec3 scaleA       = glm::vec3(100.f);
            static glm::vec3 scaleB       = glm::vec3(.5);
			{
                mdlSkull.setPosition(translationA);
                mdlSkull.setRotation(rotationA);
                mdlSkull.setScale(scaleA);
                renderer.draw(mdlSkull);
			}
			{
                mdl2.setPosition(translationB);
                mdl2.setRotation(rotationB);
                mdl2.setScale(scaleB);
                renderer.draw(mdl2);
			}
			//if (r > 1.0f) increment = -0.05f;
			//else if (r < 0.0f) increment = 0.05f;
			//r += increment;

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
				//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::SliderFloat3("Translation A", &translationA.x, -resx, resx);
                ImGui::SliderFloat3("Rotation A   ", &rotationA.x   , -360, 360);
                ImGui::SliderFloat3("Scale A      ", &scaleA.x      , -10, 10);
                ImGui::NewLine();
				ImGui::SliderFloat3("Translation B", &translationB.x, -resx, resx);
                ImGui::SliderFloat3("Rotation B   ", &rotationB.x   , -360, 360);
                ImGui::SliderFloat3("Scale B      ", &scaleB.x      , -10, 10);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);


		}
		//glDeleteProgram(shader);
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}