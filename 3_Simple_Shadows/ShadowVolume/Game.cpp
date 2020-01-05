#include "Game.h"

Game::Game(std::string* WindowTitle)
    : WINDOW_WIDTH(1200), WINDOW_HEIGHT(800), camera001( Camera(12.5f, 0.45) )
{
    this->firstMouse = true;
    this->lastX = 400;
    this->lastY = 300;

    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;

    this->angle = 0;
    this->pause = 0;

    this->initGLFW();
    this->initWindow(WindowTitle);
    this->initCallbacks();
    this->initGLEW();
    this->initShaders();
    this->initModels();
    this->initBuffers();
    this->initScreenVAO();
    this->initFloorVAO();
    this->initVars();
}


Game::~Game()
{
    TwTerminate();    
    glfwDestroyWindow(this->window);
	glfwTerminate();
	
    for( size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

    for( size_t i = 0; i<this->models.size(); i++)
        delete this->models[i];
}

void Game::run()
{
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while ( !glfwWindowShouldClose(this->window) )
    {
        /* UPDATE SCENE */
        this->updateVariables();

        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        /* RENDER SCENE */
        this->renderScene();
    }
}

void Game::initGLFW()
{
    // GLFW
	if( glfwInit() == GLFW_FALSE )
	{
		glfwTerminate();
        throw std::exception("ERROR::GLFW_INIT_FAILED  \n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	
	glfwWindowHint(GLFW_SAMPLES, 4);

    glfwSwapInterval(0);
}

void Game::initWindow(std::string* Title)
{
    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, Title->c_str(), nullptr, nullptr);
	if( this->window == nullptr)
	{
		glfwTerminate();
        throw std::exception("ERROR::GLFW_WINDOW_INIT_FAILED  \n");
	}
    
    glfwMakeContextCurrent(this->window);
}

void Game::initCallbacks()
{
    /*glfwSetCursorPosCallback(this->window, reinterpret_cast<GLFWcursorposfun>(this->cursorPositionCallback));
	glfwSetKeyCallback(this->window, reinterpret_cast<GLFWkeyfun>(this->keyCallback));
	glfwSetScrollCallback(this->window, reinterpret_cast<GLFWscrollfun>(this->scrollCallback));
	glfwSetMouseButtonCallback(this->window, reinterpret_cast<GLFWmousebuttonfun>(this->mouseButtonCallback));*/
}

void Game::initGLEW()
{
    glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK )
	{
		glfwTerminate();
        throw std::exception("ERROR::GAME::GLEW_INIT_FAILED");
	}

    glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void Game::initShaders()
{
    this->shaders.push_back( new Shader("shader/render.vert", "shader/render.frag"));
    this->shaders.push_back( new Shader("shader/volume.vert", "shader/volume.frag", "shader/volume.geom"));
    this->shaders.push_back( new Shader("shader/comp.vert", "shader/comp.frag"));
    this->shaders.push_back( new Shader("shader/plane.vert", "shader/plane.frag"));
}

void Game::initModels()
{
    this->models.push_back( new Model("Models/bunny.obj"));
}

void Game::initBuffers()
{
    this->initDepthBuffer();
    this->initAmbientBuffer();
    this->initDiffuseTexture();
    this->initFBO();
    this->initDrawBuffers();
    this->checkBuffersCreation();

    /* Clean - Unbind any buffers */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::initDepthBuffer()
{
	glGenRenderbuffers(1, &this->depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
}

void Game::initAmbientBuffer()
{
	glGenRenderbuffers(1, &this->ambBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, this->ambBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
}

void Game::initDiffuseTexture()
{
	glGenTextures(1, &this->diffSpecTex);
	glBindTexture(GL_TEXTURE_2D, this->diffSpecTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Game::initFBO()
{
    glGenFramebuffers(1, &this->colorDepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->colorDepthFBO);

    // Link previously created buffers to created Frame Buffer Object.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->ambBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->diffSpecTex, 0);
}

void Game::initDrawBuffers()
{
    this->drawBuffers[0] = GL_COLOR_ATTACHMENT0;
    this->drawBuffers[1] = GL_COLOR_ATTACHMENT1;

    /* Specifies a list of color buffers to be drawn into */
    glDrawBuffers(2, drawBuffers);
}

void Game::checkBuffersCreation()
{
	if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        throw runtime_error("Framebuffer is not complete.\n");
	}

    printf("Framebuffer is complete.\n");
}

void Game::initLights()
{
    this->lightPosition = glm::vec4(100.5, 0.1f, 0.0f, 1.0f);
}

void Game::initMatrices()
{
    this->model = glm::mat4(1.f);
    this->model = glm::translate(this->model, glm::vec3(5.f, 0.f, 0.f));	// vec3 - translation vector
    this->model = glm::scale(this->model, glm::vec3(0.f));
	this->model = glm::rotate(this->model, 1.1f, glm::vec3(0.0f, 1.0f, 0.0f));

    this->projection = glm::perspective(1.0f, (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.2f, 50.0f);

    this->view = this->camera001.getRotViewMatrix();
}

void Game::initScreenVAO()
{
    glGenBuffers(1, &this->bufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), this->ScreenVerts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &this->fsQuad);
	glBindVertexArray(this->fsQuad);

	glBindBuffer(GL_ARRAY_BUFFER, this->bufHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Game::initFloorVAO()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->FloorVertices), FloorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	
    glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Game::initVars()
{
	this->speed = 1.0f; 
	this->height = 5.f;
	this->distance = 4.0f;
	this->intensity = 1.0f;
	this->showEdge = 0;
	this->showFrustum = 0;
	this->time = 0.0f;
	this->fps = 60.0f;
	this->bar = nullptr;
}

void Game::updateVariables()
{
    this->updateMatrices();
    this->updateTime();

    /* Update FPS count */
    this->fps = 1.0f / this->deltaTime;

    /* Update Input Events */
    glfwPollEvents();

    this->updateKeyboardInput();
    this->updateMouseInput();

    if(!this->pause)
        this->UpdateLightPosition();
}

void Game::updateMatrices()
{
    this->projection = glm::perspective(1.0f, (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.1f, 50.0f);
    this->view = this->camera001.getRotViewMatrix();

    this->mv = this->view * this->model;
}

void Game::updateTime()
{
    this->time = glfwGetTime();

    this->deltaTime = this->time - this->lastFrame;
    this->lastFrame = this->time;
}

void Game::UpdateLightPosition()
{
    this->angle += this->deltaTime * this->speed;

    if (this->angle > glm::two_pi<float>()) 
	    this->angle -= glm::two_pi<float>();

    this->lightPosition = glm::vec4(
        3.0f * glm::vec3(cosf(angle) * distance, height, sinf(angle) * distance), 1.0f);

}

void Game::renderScene()
{

    /* Render whole scene into FBO .... */
    this->renderFirstPass();

    /*  */
    this->renderSecondPass();

    /*  */
    this->renderThirdPass();

	glfwSwapBuffers(window);
}

void Game::renderFirstPass()
{
    // Clear color, depth and stencil buffer.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene to FBO
	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

    /* Render First Model Using Its Shader */
    this->shaders[SHADER_RENDER]->use();
	this->shaders[SHADER_RENDER]->setMat4("ModelViewMatrix", this->view);
	this->shaders[SHADER_RENDER]->setMat4("ProjMatrix", this->projection);
	this->shaders[SHADER_RENDER]->setVec3("LightPosition", this->view * this->lightPosition);
	this->shaders[SHADER_RENDER]->setFloat("LightIntensity", this->intensity);
    this->shaders[SHADER_RENDER]->setMat3("NormalMatrix", glm::mat3(
        glm::vec3(this->mv[0]), 
        glm::vec3(this->mv[1]), 
        glm::vec3(this->mv[2])
    ));

	glBindFramebuffer(GL_FRAMEBUFFER, this->colorDepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	this->models[MODEL_BUNNY]->Draw(*this->shaders[SHADER_RENDER]);

    /* Render Floor Using Floor Shader */
	this->shaders[SHADER_PLANE]->use();
	this->shaders[SHADER_PLANE]->setMat4("ModelViewMatrix",this->view);
	this->shaders[SHADER_PLANE]->setMat4("ProjMatrix", this->projection);
	this->shaders[SHADER_PLANE]->setVec3("u_color", 0.6f, 0.6f, 0.6f);
	this->shaders[SHADER_PLANE]->setVec3("LightPosition", this->view * this->lightPosition);
	this->shaders[SHADER_PLANE]->setFloat("LightIntensity", this->intensity);
    this->shaders[SHADER_PLANE]->setMat3("NormalMatrix", glm::mat3(
        glm::vec3(this->view[0]), 
        glm::vec3(this->view[1]), 
        glm::vec3(this->view[2])
    ));

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Game::renderSecondPass()
{
    /* 
     * Create the silhouette of the model from light's point of view
     * and extend sides of calculated silhouette. 
     * Copy content of FBO buffer ( color and depth ) into the default FBO. 
     */
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->colorDepthFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 
        0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 
        GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);

	if (!this->showEdge) {
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	}
	if (!this->showFrustum) {
		glDepthMask(GL_FALSE);
	}
	glEnable(GL_DEPTH_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xff);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);

	this->shaders[SHADER_VOLUME]->use();

	this->shaders[SHADER_VOLUME]->setMat4("ModelViewMatrix", this->view);
	this->shaders[SHADER_VOLUME]->setMat4("ProjMatrix", this->projection);
	this->shaders[SHADER_VOLUME]->setVec3("LightPosition", this->view * this->lightPosition);

	this->models[MODEL_BUNNY]->Draw(*this->shaders[SHADER_VOLUME]);

	glDisable(GL_DEPTH_CLAMP);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void Game::renderThirdPass()
{
    /* Draw the diffuse component according to the stencil buffer. */
    glDisable(GL_DEPTH_TEST);

    glStencilFunc(GL_EQUAL, 0, 0xffff);

    this->shaders[SHADER_COMP]->use();

    glBindVertexArray(this->fsQuad);
    glBindTexture(GL_TEXTURE_2D, this->diffSpecTex);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
}


/* CALLBACKS FUNCTIONALITY */
void Game::updateKeyboardInput()
{
    // Application
	if( glfwGetKey( this->window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Camera
	if( glfwGetKey( this->window, GLFW_KEY_W ) == GLFW_PRESS )
	{
		//camera.move(this->dt, FORWARD);
	}

	if( glfwGetKey( this->window, GLFW_KEY_S ) == GLFW_PRESS )
	{
		//camera.move(this->dt, BACKWARD);
	}

	if( glfwGetKey( this->window, GLFW_KEY_A ) == GLFW_PRESS )
	{
		//camera.move(this->dt, LEFT);
	}

	if( glfwGetKey( this->window, GLFW_KEY_D ) == GLFW_PRESS )
	{
		//camera.move(this->dt, RIGTH);
	}

    if( glfwGetKey( this->window, GLFW_KEY_P ) == GLFW_PRESS )
	{
		this->pause = ~this->pause;
	}

	if( glfwGetKey( this->window, GLFW_KEY_X ) == GLFW_PRESS )
	{
		this->showEdge = ~this->showEdge;
	}

	if( glfwGetKey( this->window, GLFW_KEY_Z ) == GLFW_PRESS )
	{
        this->showFrustum = ~this->showFrustum;
	}
}

void Game::updateMouseInput()
{
    double curr_x;
    double curr_y;
    glfwGetCursorPos(this->window, &curr_x, &curr_y);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		this->camera001.updatePosition(curr_x, curr_y, this->firstMouse);
		this->firstMouse = false;
	} else {
		this->camera001.resetCamera();
		this->firstMouse = true;
	}
}