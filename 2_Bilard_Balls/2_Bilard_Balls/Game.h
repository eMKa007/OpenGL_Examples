#pragma once

#include "libs.h"
#include "Camera.h"
//#define DEBUG

/* ENUMERATIONS */
enum shaders_enum
{
	SHADER_BOX = 0
};

enum textureUnit_enum
{
	TEX_BACKGROUD = 0
};

enum material_enum
{
	MAT_BG = 0
};

enum mesh_enum
{
	MESH_QUAD = 0
};

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

	// Delta Time
	float dt;
	float currTime;
	float lastTime;

	//Camera
	Camera camera;

	// Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	// Shaders
	std::vector<Shader*> shaders;

	// Textures
	std::vector<Texture*> textures;

	//Models
	std::vector<Model*> models;

	// Materials
	std::vector<Material*> materials;

	// Lights
	std::vector<glm::vec3*> lights;

	/* STATIC VARIABLES */

	/* PRIVATE FUNCTIONS */
	void initGLFW();
	void initWindow( const char* title, bool resizable );
	void initGLEW(); //AFTER CONTEXT CREATION
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initModels( float SphereRadius );
	void initLights();
	void initUniforms();

	void updateUniforms();

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
	void updateInput();
	void updateKeyboardInput();
	void updateMouseInput();
	void updateDt();

	void update();
	void render();

	/* STATIC FUNCTIONS */
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH );
};
