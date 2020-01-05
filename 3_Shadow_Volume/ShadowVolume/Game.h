#pragma once

#include "libs.h"

enum SHADERS_ENUM
{
    SHADER_RENDER,
    SHADER_VOLUME,
    SHADER_COMP,
    SHADER_PLANE
};

enum MODELS_ENUM
{
    MODEL_BUNNY
};

class Game
{
public:
    Game(std::string* WindowTitle);
    ~Game();

    void run();

private:
    /* WINDOW VARIABLES */
    GLFWwindow* window;    
    const unsigned int WINDOW_WIDTH;
    const unsigned int WINDOW_HEIGHT;
    float deltaTime;
    float lastFrame;
    float time;

    /* CAMERA VARIABLES */
    Camera camera001;
    bool firstMouse;
    GLfloat lastX;
    GLfloat lastY;
    double gCursorPositionY;

    /* SHADERS */
    std::vector<Shader*> shaders;

    /* MODELS */
    std::vector<Model*> models;

    /* BUFFERS */
    GLuint depthBuf;
    GLuint ambBuf;
    GLuint diffSpecTex;
    GLuint colorDepthFBO;
    GLenum drawBuffers[2] = {0};

    /* SCREEN VARIABLES */
    GLfloat ScreenVerts[12] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
	GLuint bufHandle;
    GLuint fsQuad;

    /* FLOOR VARIABLES */
    GLfloat FloorVertices[12] = {
		15.0f, 0.95f, 15.0f,
		-15.0f, 0.95f, 15.0f,
		15.0f, 0.95f, -15.0f,
		-15.0f, 0.95f, -15.0f
	};
    GLuint VBO;
    GLuint VAO;

    /* LIGHT VARIABLES */
    glm::vec4 lightPosition;

    /* MATRICES */
    glm::mat4 model;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 mv;

    /* UI VARIABLES */
    float speed;
    float height;
    float distance;
    float intensity;
    int showEdge;
    int showFrustum;
    float fps;
    int pause;

    /* ENVIRONMENTAL VARIABLES */
    float angle;

    /* Initialize Functions */
    void initGLFW();
    void initWindow(std::string* Title);
    void initCallbacks();
    void initGLEW();
    void initShaders();
    void initModels();
    void initBuffers();
        void initDepthBuffer();
        void initAmbientBuffer();
        void initDiffuseTexture();
        void initFBO();
        void initDrawBuffers();
        void checkBuffersCreation();
    void initLights();
    void initMatrices();
    void initScreenVAO();
    void initFloorVAO();
    void initVars();

    /* Update Functions */
    void updateVariables();
    void updateMatrices();
    void updateTime();
    void UpdateLightPosition();

    /* Render Functions */
    void renderScene();
    void renderFirstPass();
    void renderSecondPass();
    void renderThirdPass();

    /* Callbacks functions */
    void updateKeyboardInput();
    void updateMouseInput();
};
