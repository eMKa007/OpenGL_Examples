
#include "libs.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

float x = 1.0f;
float y = 1.0f;
float z = 1.0f;

Vertex vertices[] =
{
	// Position						// Color					// texcoord				// Normal
	// Triangle Front
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2

	// Triangle Left
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 0
		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(0.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 1
		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 2

	// Triangle Back
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 0
		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 1
		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 2

	// Triangle Right
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 0
		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(0.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 1
		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 2	

	// Triangle Floor
		glm::vec3(-10.f, -1.5f, -10.f),	glm::vec3(0.5f, 0.f, 0.5f),	glm::vec2(0.f, 1),		glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-10.f, -1.5f, 10.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(10.f, -1.5f, 10.f),	glm::vec3(0.5f, 0.f, 0.5f),	glm::vec2(1, 0.f),		glm::vec3(0.f, 0.f, 1.f),	// Vertex 2

		glm::vec3(-10.f, -1.5f, -10.f),	glm::vec3(0.5f, 0.f, 0.5f),	glm::vec2(0.f, 1),		glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(10.f, -1.5f, 10.f),	glm::vec3(0.5f, 0.f, 0.5f),	glm::vec2(1, 0.f),		glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(10.f, -1.5f, -10.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1, 1),		glm::vec3(0.f, 0.f, 1.f)	// Vertex 3

};

GLuint indices[10] =
{
};

GLFWwindow* window;
const char* APP_TITLE = "3_Simple_Shadows";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
int framebufferWidth;
int framebufferHeight;

glm::vec3 camPosition;
glm::vec3 camFront;
glm::vec3 worldUp;

GLuint VAO;
GLuint VBO;
GLuint EBO;

glm::vec3 currPosition;

int main(int argc, char* argv[] )
{
	/* Init GLFW */
	if( glfwInit() == GLFW_FALSE )
	{
		std::cout << "ERROR::GLFW_INIT_FAILED  \n";
		glfwTerminate();
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);

	if( window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED  \n";
		glfwTerminate();
		return 0;
	}
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( err != GLEW_OK )
	{
		std::cout << "ERROR::GAME::GLEW_INIT_FAILED" << std::endl;
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
		return 0;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// Init input options

	worldUp = glm::vec3(0.f, 1.f, 0.f);
	camFront = glm::vec3(-1.f, -1.5f, -1.f);
	camPosition = glm::vec3(3.f, 3.f, 3.f);
	
	glm::mat4 ViewMatrix = glm::mat4(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	glm::mat4 ProjectionMatrix = glm::mat4(1.f);

	ProjectionMatrix = glm::perspective(
		glm::radians(90.f),
		static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight), 
		0.1f, 
		100.f
	);

	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);
	unsigned nrOfIndices = 0;//sizeof(indices) / sizeof(GLuint);

	Shader* Program = new Shader( 4, 4,
		"vertex_core.glsl", "fragment_core.glsl");

	currPosition = glm::vec3(0.f);

	/* Initialize  Meshes */
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray( VAO );

	glGenBuffers(1, &VBO);
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData(GL_ARRAY_BUFFER, nrOfVertices*sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);

		// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);	// Need to be same number (location) as inside vertex shader (used by core_program).

		// Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);	// Need to be same number (location) as inside vertex shader (used by core_program).

		// Texture Coordinates Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);	// Need to be same number (location) as inside vertex shader (used by core_program).

		// Normal Vectors Attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray( 0 );

	/* Update Model matrix to this mesh */
	glm::mat4 ModelMatrix = glm::mat4(1.f);

	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

	//Init Uniforms
	Program->setMat4fv(ViewMatrix, "ViewMatrix");
	Program->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		
    	glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.

		ViewMatrix = glm::mat4(1.f);
		ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(
		glm::radians(90.f),
		static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight), 
		0.1f, 
		100.f
		);


		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::translate(ModelMatrix,   currPosition - glm::vec3(0.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

		Program->setMat4fv(ViewMatrix, "ViewMatrix");
		Program->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
		Program->setMat4fv(ModelMatrix, "ModelMatrix");

		glBindVertexArray( VAO );

		Program->use();

		if (nrOfIndices == 0 )
			glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glFlush();

		// Unbind the current program
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
	}

    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}