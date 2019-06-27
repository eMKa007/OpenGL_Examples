#include "libs.h"

void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight );

int main()
{
	/* Init GLFW */
	if(!glfwInit()) {
    return -1;
	}

	/* Create Window */
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	//Use OpenGL core profile, OpenGL version 4.4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);		
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Main_Window", NULL, NULL);

	// Created frame buffer is the same as window.
	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	// Canvas size. From (0,0) to (framebufferWidth, framebufferHeight)
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Switched to resizeable window
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
		
	// Important!!! Bind created window to thread.
	glfwMakeContextCurrent(window);


	/* Init GLEW (Needs the window and OpenGL context) */
	glewExperimental = GL_TRUE;

	//Error
	if( glewInit() != GLEW_OK )
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}


	/* Main Loop */
	while( !glfwWindowShouldClose(window) )
	{
		/* CHECK INPUT */
		glfwPollEvents();

		/* UPDATE */

		/* DRAW */
			// Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.

			// Draw

			// End Draw
		glfwSwapBuffers(window);
		glFlush();
	}

	/* End of program */
	glfwTerminate();

	return 0;
}

void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight )
{
	glViewport(0, 0, framebufferWidth, framebufferHeight);
}