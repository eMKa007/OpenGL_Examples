#include "Game.h"

/* ---------------------------- CONSTRUCTORS/DESTRUCTORS ---------------------------- */

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: const char* title - title of newly created game window
*		const int WINDOW_WIDTH - created window width in px
*		const int WINDOW_HEIGHT - created window height in px
*		const int GL_VERSION_MAJOR - OpenGL major version
*		const int GL_VERSION_MINOR - OpenGL minor version
*		bool resizable - is window resizeable, default value is true
*		Used to: delete previously created primitive instance
*/
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

	this->dt = 0.0f;
	this->currTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

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



/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*		Used to: delete previously created game class instance 
*		and delete all its resources
*/
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



/* ---------------------------- INITIALIZE FUNCTIONS ---------------------------- */

/*	----------------------------------------------------------
*	Function name: initGLFW()
*	Parameters:	none
*	Used to: initialize GLFW, need to be called first.
*	Return:	void
*/
void Game::initGLFW()
{
	/* Init GLFW */
	if( glfwInit() == GLFW_FALSE )
	{
		std::cout << "ERROR::GLFW_INIT_FAILED  \n";
		glfwTerminate();
	}
}



/*	----------------------------------------------------------
*	Function name: initWindow()
*	Parameters:	const char* title, bool resizable
*	Used to: Create new window with given title. 
*	Return:	void
*/
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

	// Switched to resizeable window
	if(resizable)
		glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);
		
	// Important!!! Bind created window to thread. !!!
	glfwMakeContextCurrent(this->window);
}



/*	----------------------------------------------------------
*	Function name: initGLEW()
*	Parameters:	none
*	Used to: Initialize GLEW. 
*	Return:	void
*/
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



/*	----------------------------------------------------------
*	Function name: initOpenGLOptions()
*	Parameters:	none
*	Used to: Set specific OpenGL options [cull_face, polygonMode, blend]. 
*	Return:	void
*/
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Color blending function. 

	// Init input options
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}



/*	----------------------------------------------------------
*	Function name: initMatrices()
*	Parameters:	none
*	Used to: Initialize matrices used to render scene. 
*	Return:	void
*/
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



/*	----------------------------------------------------------
*	Function name: initShaders()
*	Parameters:	none
*	Used to: Create and compile shaders.
*	Return:	void
*/
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



/*	----------------------------------------------------------
*	Function name: initShaders()
*	Parameters:	none
*	Used to: Create texture objects.
*	Return:	void
*/
void Game::initTextures()
{
	/* INIT TEXTURE 0 */
	this->textures.push_back(
		new Texture("Images/atom.png", 
			GL_TEXTURE_2D)
	);

	this->textures.push_back(
		new Texture("Images/atom_specular.png", 
			GL_TEXTURE_2D)
	);

	/* INIT TEXTURE 1 */
	this->textures.push_back(
		new Texture("Images/floor.png", 
			GL_TEXTURE_2D)
	);

	this->textures.push_back(
		new Texture("Images/floor_specular.png", 
			GL_TEXTURE_2D)
	);
}



/*	----------------------------------------------------------
*	Function name: initShaders()
*	Parameters:	none
*	Used to: Create material objects. Ambient/diffuse/specular are set.
*	Return:	void
*/
void Game::initMaterials()
{
	/* INIT MATERIAL OBJECTS */
	this->materials.push_back( 
		new Material (glm::vec3(0.1f), 
			glm::vec3(1.f), 
			glm::vec3(1.f), 
			0, 
			1)
	);

}



/*	----------------------------------------------------------
*	Function name: initMeshes()
*	Parameters:	none
*	Used to: Create Mesh objects to hold primitives used by render.
*	Return:	void
*/
void Game::initMeshes()
{
	/* MODEL MESH */
	this->meshes.push_back( 
		new Mesh( &Pyramid(),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f) )
	);
}



/*	----------------------------------------------------------
*	Function name: initLights()
*	Parameters:	none
*	Used to: Determine lights position.
*	Return:	void
*/
void Game::initLights()
{
	// LIGHTS
	this->lights.push_back( new glm::vec3 (0.f, 0.f, 1.f) );
}



/*	----------------------------------------------------------
*	Function name: initUniforms()
*	Parameters:	none
*	Used to: Initialize/Update specific shader uniforms.
*	Return:	void
*/
void Game::initUniforms()
{
	//Init Uniforms
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
}



/* ---------------------------- PRIVATE FUNCTIONS ---------------------------- */

/*	----------------------------------------------------------
*	Function name: updateUniforms()
*	Parameters:	none
*	Used to: Update values of specific shader uniforms. 
*	Return:	void
*/
void Game::updateUniforms()
{
	// update View Matrix as we'll move the camera
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

	// Update Camera Position
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPosition");

	// Update frame buffers size, and send new Projection Matrix.
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective
	(
		glm::radians(this->fov), 
		static_cast<float>(this->framebufferWidth)/this->framebufferHeight, 
		this->nearPlane,
		this->farPlane
	);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}



/* ---------------------------- PUBLIC FUNCTIONS ---------------------------- */

/*	----------------------------------------------------------
*	Function name: getWindodShouldClose()
*	Parameters:	none
*	Used to: Determine if specific window should be closed. 
*	Return:	void
*/
int Game::getWindodShouldClose()
{
	return glfwWindowShouldClose(this->window);
}



/*	----------------------------------------------------------
*	Function name: setWindowShouldClose()
*	Parameters:	none
*	Used to: Set close flag of specific window to true.
*	Return:	void
*/
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}



/*	Function name: updateInput()
*	Parameters:	none
*	Used to: Update input to interact with rendered scene.
*	Return:	void
*/
void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
}



/*	Function name: updateInput()
*	Parameters:	none
*	Used to: Update keyboard input to interact with rendered scene.
*	Return:	void
*/
void Game::updateKeyboardInput()
{
	// Application
	if( glfwGetKey( this->window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		this->setWindowShouldClose();
	}

	// Camera
	if( glfwGetKey( this->window, GLFW_KEY_W ) == GLFW_PRESS )
	{
		this->camPosition.z -= 0.05f;
	}

	if( glfwGetKey( this->window, GLFW_KEY_S ) == GLFW_PRESS )
	{
		this->camPosition.z += 0.05f;
	}

	if( glfwGetKey( this->window, GLFW_KEY_A ) == GLFW_PRESS )
	{
		this->camPosition.x -= 0.05f;
	}

	if( glfwGetKey( this->window, GLFW_KEY_D ) == GLFW_PRESS )
	{
		this->camPosition.x += 0.05f;
	}

	if( glfwGetKey( this->window, GLFW_KEY_SPACE ) == GLFW_PRESS )
	{
		this->camPosition.y += 0.05f;
	}

	if( glfwGetKey( this->window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS )
	{
		this->camPosition.y -= 0.05f;
	}
}



/*	Function name: updateMouseInput()
*	Parameters:	none
*	Used to: Update mouse input to interact with rendered scene.
*	Return:	void
*/
void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if( this->firstMouse )
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	// Calculate Offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->mouseY - this->lastMouseY;

	// Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}



/*	Function name: updateDt()
*	Parameters:	none
*	Used to: Update delta time.
*	Return:	void
*/
void Game::updateDt()
{
	this->currTime = static_cast<float>(glfwGetTime());
	this->dt = this->currTime - this->lastTime;
	this->lastTime = this->currTime;
}



/*	----------------------------------------------------------
*	Function name: update()
*	Parameters:	none
*	Used to: Catch any input through GLFW. Calls periodically. 
*	Return:	void
*/
void Game::update()
{
	this->updateDt();

	/* CHECK INPUT */
	this->updateInput();

	this->meshes[0]->rotate(glm::vec3(0.f, 1.f, 0.f));

#ifdef DEBUG
	std::cout << "DT: " << this->dt << "; Mouse offsetX: " << this->mouseOffsetX  <<  "; offsetY: "<< this->mouseOffsetY << std::endl;
#endif
}



/*	----------------------------------------------------------
*	Function name: render()
*	Parameters:	none
*	Used to: Render whole scene inside previously created window. Calls periodically.
*	Return:	void
*/
void Game::render()
{
	/* DRAW */
		// Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.

	/* ---------------   START OF CURRENT CORE_PROGRAM --------------- */

	// Update uniforms (variables send to gpu [shader] from cpu)- every change they're updated.
	this->updateUniforms();

	// Update Uniforms
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

		// Use a program (shader) - need to tell what shader we want to use.
	this->shaders[SHADER_CORE_PROGRAM]->use();

		// Activate Texture
	this->textures[TEX_CONTAINER1]->bind(0);
	this->textures[TEX_CONTAINER1_SPECULAR]->bind(1);

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



/* ---------------------------- STATIC FUNCTIONS ---------------------------- */

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

