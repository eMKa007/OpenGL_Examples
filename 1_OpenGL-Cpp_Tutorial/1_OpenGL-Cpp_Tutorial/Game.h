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

	/* STATIC VARIABLES */

	/* PRIVATE FUNCTIONS */
	void initGLFW();
	void initWindow( const char* title, bool resizable );
	void initGLEW(); //AFTER CONTEXT CREATION

public:
	/* CONSTRUCTORS/DESTRUCTORS */
	Game(const char* title, 
		const int width, const int height,
		const int GLmajorVer, const int GLminorVer,
		bool resizable = true);
	virtual ~Game();
	
	/* ACCESSORS */

	/* MODIFIERS */

	/* FUNCTIONS */
	void update();
	void render();

	/* STATIC FUNCTIONS */
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH );
};
