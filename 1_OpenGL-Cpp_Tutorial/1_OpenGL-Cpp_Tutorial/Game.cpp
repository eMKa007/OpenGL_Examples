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

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / static_cast<float>(this->framebufferHeight), 
		this->nearPlane, 
		this->farPlane
	);
}

void Game::initShaders()
{
	/* SHADER INIT
	 * Create and compile shaders.
	 */
	this->shaders.push_back(
		new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, 
			"vertex_core.glsl", 
			"fragment_core.glsl")
	);
}

void Game::initTextures()
{
	/* INIT TEXTURE 0 */
	this->textures.push_back(
		new Texture("Images/atom.png", 
			GL_TEXTURE_2D, 
			GL_TEXTURE0)
	);

	/* INIT TEXTURE 1 */
	this->textures.push_back(
		new Texture("Images/floor.png", 
			GL_TEXTURE_2D, 
			GL_TEXTURE1)
	);
}

void Game::initMaterials()
{
	/* INIT MATERIAL OBJECTS */
	this->materials.push_back( 
		new Material (glm::vec3(0.1f), 
			glm::vec3(1.f), 
			glm::vec3(1.f), 
			this->textures[TEX_ATOM0]->getTextureUnit(), 
			this->textures[TEX_CONTAINER1]->getTextureUnit())
	);

}

void Game::initMeshes()
{
	/* MODEL MESH */
	this->meshes.push_back( 
		new Mesh( &Quad(),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f) )
	);
}

void Game::initLights()
{
	// LIGHTS
	this->lights.push_back( new glm::vec3 (0.f, 0.f, 1.f) );
}

void Game::initUniforms()
{
	//Init Uniforms
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPosition");
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

	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);
	this->camPosition = glm::vec3(0.f, 0.f, 1.f);

	this->fov = 90.f;	// Field of view. 90 degrees.
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMeshes();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for( size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for( size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for( size_t i = 0; i < this->meshes.size(); i++)
		delete this->meshes[i];

	for( size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for( size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];
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
	/* CHECK INPUT */
	glfwPollEvents();
}

void Game::render()
{
	/* UPDATE */
	//updateInput(window);

	/* DRAW */
		// Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.

	/* ---------------   START OF CURRENT CORE_PROGRAM --------------- */
		// Update uniforms (variables send to gpu [shader] from cpu)- every change they're updated.
	this->shaders[SHADER_CORE_PROGRAM]->set1i( this->textures[TEX_ATOM0]->getTextureUnit(), "texture0");
	this->shaders[SHADER_CORE_PROGRAM]->set1i( this->textures[TEX_CONTAINER1]->getTextureUnit(), "texture1");
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	
			// Update frame buffers size, and send new Projection Matrix.
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	
	ProjectionMatrix = glm::perspective
	(
		glm::radians(fov), 
		static_cast<float>(framebufferWidth)/framebufferHeight, 
		nearPlane, 
		farPlane
	);
	
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	
		// Use a program (shader) - need to tell what shader we want to use.
	this->shaders[SHADER_CORE_PROGRAM]->use();

		// Activate Texture
	this->textures[TEX_ATOM0]->bind();
	this->textures[TEX_CONTAINER1]->bind();

		// Draw
	this->meshes[MESH_QUAD]->render( this->shaders[SHADER_CORE_PROGRAM] );

		// End Draw
	glfwSwapBuffers(window);
	glFlush();

		// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D,0);

	/* ---------------   END OF CURRENT CORE_PROGRAM --------------- */
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
