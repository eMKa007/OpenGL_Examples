#include "Game.h"
#include <sstream>

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
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(6.f,10.f, 10.f), glm::vec3(-30.f, -120.f, 0.f), glm::vec3(0.f, 1.f, 0.f))
{
	this->window = nullptr;
	this->framebufferWidth = WINDOW_WIDTH;
	this->framebufferHeight = WINDOW_HEIGHT;

	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);
	this->camPosition = glm::vec3(0.f, 0.f, 1.f);

	this->fov = 45.f;	// Field of view. 90 degrees.
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

    this->angle = 0.0f;
    this->updateLight = true;

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
	this->initLights();
	this->initModels( 0.2f );
	this->initUniforms();

	this->initDephMapFrameObject();

	//glfwSwapInterval(1);
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

	for( size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for( auto *&i : this->models)
		delete i;

	for( size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];

	delete DepthMapFBO;
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
			"vertex_shader_box.vert", 
			"fragment_shader_box.frag"));

	this->shaders.push_back(
	new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_shader_sphere.vert",
		"fragment_shader_sphere.frag"));

	this->shaders.push_back(
	new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_shader_floor.vert",
		"fragment_shader_floor.frag"));

	this->shaders.push_back(
		new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
			"vertex_shader_core.vert",
			"fragment_shader_core.frag"));

	this->shaders.push_back(
		new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
			"vertex_shadowMap.vert",
			"fragment_shadowMap.frag"));
}



/*	----------------------------------------------------------
*	Function name: initTextures()
*	Parameters:	none
*	Used to: Create texture objects.
*	Return:	void
*/
void Game::initTextures()
{
	this->textures.push_back(
		new Texture("Images/texture1.bmp", GL_TEXTURE_2D));

	this->textures.push_back(
		new Texture("Images/floor_specular.png", GL_TEXTURE_2D));
}



/*	----------------------------------------------------------
*	Function name: initMaterials()
*	Parameters:	none
*	Used to: Create material objects. Ambient/diffuse/specular are set.
*	Return:	void
*/
void Game::initMaterials()
{
	// Material BOX
	materials.push_back( new Material(
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
		0, 0));

	// Material SPHERES
	materials.push_back( new Material(
	glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 0));

	// Material FLOOR
	materials.push_back( new Material(
	glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
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
	this->lights.push_back( new glm::vec3 (10.f, 6.f, 10.f) );
}


/*	----------------------------------------------------------
*	Function name: initModels()
*	Parameters:	none
*	Used to: Create Model objects.
*	Return:	void
*/
void Game::initModels(float sphereRadius)
{
	/* Init random Number generator */
	std::random_device rd;
	std::mt19937 eng(rd()); //seed the generator
	std::uniform_real_distribution<float> distr(-0.1f, 0.1f); //define the range

	/* Initialize BOX */
	std::vector<Mesh*> meshes;
	meshes.push_back(
		new Mesh( &Box() ));

	models.push_back( new Model(
		glm::vec3(0.f),
		this->materials[MAT_BOX],
		nullptr,
		nullptr,
		meshes));

	meshes.clear();

	/* Initialize SPHERES */
	// Sphere 1
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(1.f)));

	// Sphere 2
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(0.f)));

	// Sphere 3
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(-1.f)));

	// Sphere 4
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(-1.f, 1.f, -1.f)));

	// Sphere 5
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(-1.f, 1.f, 1.f)));

	// Sphere 6
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(-1.f, -1.f, 1.f)));

	// Sphere 7
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(1.f, 1.f, -1.f)));

	// Sphere 8
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(1.f, -1.f, -1.f)));

	// Sphere 9
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius, 36, 18),
		glm::vec3(distr(eng), distr(eng), distr(eng)),
		glm::vec3(1.f, -1.f, 1.f)));

	models.push_back( new Model(
		glm::vec3(0.f),
		this->materials[MAT_SPHERES],
		nullptr,
		nullptr,
		meshes));

	meshes.clear();

	meshes.push_back( new Mesh( &Quad( 5.f ), 
		glm::vec3(0.f), 
		glm::vec3(0.f, -2.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(-90.f, 0.f, 0.f),
		glm::vec3(50.f)));

	models.push_back( new Model(
	glm::vec3(0.f), 
		this->materials[MAT_FLOOR],
		this->textures[TEX_FLOOR],
		this->textures[TEX_FLOOR_SPECULAR], 
		meshes));

	meshes.clear();

	// position is pushed backwards to simulate sun object.
	meshes.push_back( new Mesh( 
		&Sphere(sphereRadius*5, 36, 18),
		glm::vec3(0.f), (*this->lights[0]) * 5.f));

	models.push_back( new Model(
		glm::vec3(0.f),
		this->materials[MAT_SPHERES],
		nullptr,
		nullptr, 
		meshes));

	// Remove unnecessary meshes
	for( auto *& i : meshes )
	{
		delete i;
	}

    /* Initialize Assimp Models */

    std::unique_ptr<AssimpLoader> SpiderModel = std::make_unique<AssimpLoader>("Models/bunny.obj", 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals );
    const std::vector<Mesh*> spiderMeshes = SpiderModel->GetMeshes();

    models.push_back( new Model(
		glm::vec3(0.f),
		this->materials[MAT_SPHERES],
		this->textures[TEX_FLOOR],
		nullptr, 
		spiderMeshes));

    this->models[MODEL_SPIDER]->setScale(glm::vec3(25.f));
    this->models[MODEL_SPIDER]->rotate(glm::vec3(0.f, 90.f, 0.f));
    this->models[MODEL_SPIDER]->moveMeshes( glm::vec3(0.f, -3.f, -5.f));
}

/*	----------------------------------------------------------
*	Function name: initDephMapFrameObject()
*	Parameters:	none
*	Used to: Create depth map framebuffer object.
*	Return:	void
*/
void Game::initDephMapFrameObject()
{
	this->DepthMapFBO = new ShadowMapFBO(WINDOW_WIDTH, WINDOW_HEIGHT);
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
	for( auto& i : this->shaders)
	{
		i->setMat4fv(this->ViewMatrix, "ViewMatrix");
		i->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	}
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
	this->ViewMatrix = this->camera.getViewMatrix();

	// Update frame buffers size, and Projection Matrix.
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	this->ProjectionMatrix = glm::perspective
	(
		glm::radians(this->fov), 
		static_cast<float>(this->framebufferWidth)/this->framebufferHeight, 
		this->nearPlane,
		this->farPlane
	);

	this->SendUniformsToShaders();


}

void Game::updateUniforms_LightPOV()
{
	// Get View Matrix from Light POV.
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt( *(this->lights[0]), 
		glm::vec3(0.f), 
		glm::vec3(0.f, 1.0f, 0.f));

	// Update frame buffers size, and Projection Matrix.
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	this->ProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, this->nearPlane, this->farPlane); 

	this->LightSpaceMatrix = ProjectionMatrix * this->ViewMatrix; 
	this->SendUniformsToShaders();
}

void Game::SendUniformsToShaders()
{
	for( auto& i : this->shaders )
	{
		if( glGetUniformLocation( i->getID(), "ViewMatrix") != -1)
			i->setMat4fv(this->ViewMatrix, "ViewMatrix");

		if( glGetUniformLocation( i->getID(), "ProjectionMatrix") != -1)
			i->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

		if( glGetUniformLocation( i->getID(), "cameraPosition") != -1)
			i->setVec3f(this->camera.getPosition(), "cameraPosition");

		if( glGetUniformLocation( i->getID(), "lightPos0") != -1)
			i->setVec3f(*this->lights[0], "lightPos0");

		if( glGetUniformLocation( i->getID(), "LightSpaceMatrix") != -1)
			i->setMat4fv(this->LightSpaceMatrix, "LightSpaceMatrix");
	}
}

void Game::RenderFromLightPOV()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->DepthMapFBO->BindForWriting();
	
	glClear( GL_DEPTH_BUFFER_BIT);

	/* ---------------   START OF CURRENT SPHERES_PROGRAM --------------- */
	//glCullFace(GL_FRONT);
	//this->models[MODEL_SPHERES]->render(this->shaders[SHADER_SHADOW], GL_TRIANGLES);
	//glCullFace(GL_BACK); // don't forget to reset original culling face

	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT SPHERES_PROGRAM --------------- */


    /* ---------------   START OF CURRENT SPIDER_PROGRAM --------------- */
	glCullFace(GL_FRONT);
	this->models[MODEL_SPIDER]->render(this->shaders[SHADER_SHADOW], GL_TRIANGLES);
	glCullFace(GL_BACK); // don't forget to reset original culling face

	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT SPHERES_PROGRAM --------------- */


	/* ---------------   START OF CURRENT FLOOR_PROGRAM --------------- */
	this->models[MODEL_FLOOR]->render(this->shaders[SHADER_SHADOW], GL_TRIANGLES);
	
	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT FLOOR_PROGRAM --------------- */

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::RenderFromCameraPOV()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DepthMapFBO->BindForReading(2);

	/* ---------------   START OF CURRENT BOX_PROGRAM --------------- */
	//this->models[MODEL_BOX]->render(this->shaders[SHADER_BOX], GL_LINES, this->DepthMapFBO);

	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT BOX_PROGRAM --------------- */	


	/* ---------------   START OF CURRENT SPHERES_PROGRAM --------------- */
	//this->models[MODEL_SPHERES]->render(this->shaders[SHADER_SPHERES], GL_TRIANGLES, this->DepthMapFBO);

	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT SPHERES_PROGRAM --------------- */
	
	
	/* ---------------   START OF CURRENT FLOOR_PROGRAM --------------- */
	this->models[MODEL_FLOOR]->render(this->shaders[SHADER_FLOOR], GL_TRIANGLES, this->DepthMapFBO);
	
	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT FLOOR_PROGRAM --------------- */


    /* ---------------   START OF CURRENT SPIDER_PROGRAM --------------- */
	this->models[MODEL_SPIDER]->render(this->shaders[SHADER_SPHERES], GL_TRIANGLES, this->DepthMapFBO);
	
	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT FLOOR_PROGRAM --------------- */


	/* ---------------   START OF CURRENT LIGHT_SPHERE_PROGRAM --------------- */
	this->models[MODEL_LIGHT_SPHERE]->render(this->shaders[SHADER_BOX], GL_TRIANGLES);
	
	// Unbind the current program
	glBindVertexArray(0);
	glUseProgram(0);
	/* ---------------   END OF CURRENT FLOOR_PROGRAM --------------- */
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
	this->camera.updateInput(this->dt, -1, this->mouseOffsetX, this->mouseOffsetY);

    /* Update light position */
    if( updateLight )
    {
        this->angle += this->dt * 0.7f;

        if (this->angle > glm::two_pi<float>()) 
            this->angle -= glm::two_pi<float>();


        float new_x = std::cosf(this->angle) * 7.f;
        float new_z = std::sinf(this->angle) * 7.f;

        this->lights[0]->x = new_x;
        this->lights[0]->z = new_z;
    }
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
		camera.move(this->dt, FORWARD);
	}

	if( glfwGetKey( this->window, GLFW_KEY_S ) == GLFW_PRESS )
	{
		camera.move(this->dt, BACKWARD);
	}

	if( glfwGetKey( this->window, GLFW_KEY_A ) == GLFW_PRESS )
	{
		camera.move(this->dt, LEFT);
	}

	if( glfwGetKey( this->window, GLFW_KEY_D ) == GLFW_PRESS )
	{
		camera.move(this->dt, RIGTH);
	}

	if( glfwGetKey( this->window, GLFW_KEY_SPACE ) == GLFW_PRESS )
	{
		camera.move(this->dt, UPWARD);
	}

	if( glfwGetKey( this->window, GLFW_KEY_C ) == GLFW_PRESS )
	{
		camera.move(this->dt, DOWNWARD);
	}

    if( glfwGetKey( this->window, GLFW_KEY_P ) == GLFW_PRESS )
	{
		this->updateLight = !this->updateLight;
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

void Game::run()
{
    double lastTime = glfwGetTime();
    this->nbFrames = 0;

    while( !this->getWindodShouldClose() )
    {
        this->update();
        this->render();

        double currentTime = glfwGetTime();
        this->nbFrames++;
        if( currentTime - lastTime >= 1.0 )
        {
            float oneFrameTime = 1000.f/static_cast<double>(this->nbFrames);
            float FPS = 1000.f/oneFrameTime;
            printf("%.3f ms/frame --- ~%.1f FPS\n", oneFrameTime, FPS);
            this->nbFrames = 0;
            lastTime += 1.0;
        }
    }
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

	/* UPDATE SPHERES POSITION */
	this->models[MODEL_SPHERES]->move();

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
	/* CLEAR */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.
	
	/* RENDER FROM LIGHT POV */
	this->updateUniforms_LightPOV();
	this->RenderFromLightPOV();

	/* RENDER FROM CAMERA POV */
	this->updateUniforms();
	this->RenderFromCameraPOV();

	// End Draw
	glfwSwapBuffers(window);
	glFlush();

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

