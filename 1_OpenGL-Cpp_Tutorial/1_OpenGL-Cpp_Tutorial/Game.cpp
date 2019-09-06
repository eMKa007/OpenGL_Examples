#include "Game.h"


void Game::initGLFW()
{
	/* Init GLFW */
	if( glfwInit() == GLFW_FALSE )
	{
		std::cout << "ERROR::GLFW_INIT_FAILED  \n";
		glfwTerminate();
	}
		
}

void Game::initWindow( const char* title, bool resizable)
{
	//Use OpenGL core profile, OpenGL version 4.4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow( this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if( this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED  \n";
		glfwTerminate();
	}
	// Created frame buffer is the same as window. Created window is not resizeable
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	// Canvas size. From (0,0) to (framebufferWidth, framebufferHeight)
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Switched to resizeable window
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);
		
	// Important!!! Bind created window to thread. !!!
	glfwMakeContextCurrent(this->window);

}

void Game::initGLEW()
{
	/* Init GLEW (Needs the window and OpenGL context) */
	glewExperimental = GL_TRUE;

	//Error
	if( glewInit() != GLEW_OK )
	{
		std::cout << "ERROR::GAME::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	/* OPENGL OPTIONS  
	 * OpenGL is a state machine. Before we draw something, we want to enable some options.
	 */
	glEnable(GL_DEPTH_TEST); // Lock Z-coordinate. Impossible to use Z-values.

	glEnable(GL_CULL_FACE); // Remove (do not draw) object that are not seen. (Face culling)
	glCullFace(GL_BACK);	// Back side of object will not be drawn
	glFrontFace(GL_CCW);	// Front face- which will be drawn - is that with counter-clock wise vertex order. 

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); // Fill drawn shape with full color. Could be GL_LINE etc.

	glEnable(GL_BLEND); // Enable color blending.
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Color blending function. 

}

/* CONSTRUCTORS/DESTRUCTORS */
Game::Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable 
	) : 
	WINDOW_WIDTH(WINDOW_WIDTH), 
	WINDOW_HEIGHT(WINDOW_HEIGHT), 
	GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
	GL_VERSION_MINOR(GL_VERSION_MINOR)
{
	this->window = nullptr;
	this->framebufferWidth = WINDOW_WIDTH;
	this->framebufferHeight = WINDOW_HEIGHT;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

int Game::getWindodShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::update()
{
	
}

void Game::render()
{

}


/*	----------------------------------------------------------
*	Function name:	framebuffer_resize_callback()
*	Parameters:	GLFWwindow* window - window to be used in resize
*			int fbW - new frame buffer width
*			int fbH - new frame buffer height
*	Used to:		Set new size of viewport baser on given width and height values.
*	Return:		none
*/
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}
