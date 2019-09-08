#pragma once

#include "libs.h"

class Game
{
private:
	/* VARIABLES */
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	/* STATIC VARIABLES */

	/* PRIVATE FUNCTIONS */
	void initGLFW();
	void initWindow( const char* title, bool resizable );
	void initGLEW(); //AFTER CONTEXT CREATION
	void initOpenGLOptions();
	void initMatrices();

public:
	/* CONSTRUCTORS/DESTRUCTORS */
	Game(const char* title, 
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable = true);
	virtual ~Game();
	
	/* ACCESSORS */
	int getWindodShouldClose();

	/* MODIFIERS */
	void setWindowShouldClose();

	/* FUNCTIONS */
	void update();
	void render();

	/* STATIC FUNCTIONS */
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH );
};
