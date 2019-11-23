#pragma once

#include "libs.h"
#include "Camera.h"
//#define DEBUG

/* ENUMERATIONS */
enum shaders_enum
{
	SHADER_BOX = 0,
	SHADER_SPHERES,
	SHADER_FLOOR
};

enum textureUnit_enum
{
	TEX_FLOOR = 0,
	TEX_FLOOR_SPECULAR
};

enum material_enum
{
	MAT_BOX = 0,
	MAT_SPHERES,
	MAT_FLOOR
};

enum mesh_enum
{
	MESH_QUAD = 0
};

enum model_enum
{
	MODEL_BOX = 0,
	MODEL_SPHERES,
	MODEL_FLOOR,
	MODEL_LIGHT_SPHERE,
    MODEL_SPIDER
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


	// Light Space Matrix
	glm::mat4 LightSpaceMatrix;
	
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
	void initLights();
	void initModels( float SphereRadius );
	void initUniforms();

	void updateUniforms();
	void RenderFromCameraPOV();

	void SendUniformsToShaders();

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
