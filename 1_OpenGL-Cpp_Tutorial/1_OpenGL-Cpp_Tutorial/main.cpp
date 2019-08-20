#include "libs.h"

void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight );
bool loadShaders( GLuint& program );
GLuint CreateAndBindTexture( const char* src_path );
void updateInput( GLFWwindow* window )
{
	if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void updateInput( GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
	{
		position.z += -0.05f;
	}

	if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
	{
		position.x += -0.05f;	
	}

	if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
	{
		position.z += 0.05f;	
	}

	if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS )
	{
		position.x += 0.05f;	
	}

	if( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS )
	{
		rotation.y += -1.f;	
	}

	if( glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS )
	{
		rotation.y += 1.f;	
	}

	if( glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS )
	{
		scale += 0.1f;
	}

	if( glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS )
	{
		scale += -0.1f;
	}
}

/* 
 * Declare vertex positions/color/texture coordinates.
 * Will be used to draw triangle in this case.
 */
Vertex vertices[] =
{
	// Position						// Color					// texcoord				// Normal
	glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
	glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
	glm::vec3(0.5f, -0.5f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
	glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f)	// Vertex 3
	
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

/* 
 * Determine the sequence of vertices to be drawn.
 */
GLuint indices[] =
{
	0, 1, 2,	// Triangle 1
	0, 2, 3		// Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);


int main( int argc, char* argv[] )
{
	/* Init GLFW */
	if( !glfwInit() ) 
		return -1;

	/* Create Window */
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	//Use OpenGL core profile, OpenGL version 4.4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Main_Window", NULL, NULL);

	// Created frame buffer is the same as window. Created window is not resizeable
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	// Canvas size. From (0,0) to (framebufferWidth, framebufferHeight)
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Switched to resizeable window
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
		
	// Important!!! Bind created window to thread. !!!
	glfwMakeContextCurrent(window);

	/* Init GLEW (Needs the window and OpenGL context) */
	glewExperimental = GL_TRUE;

	//Error
	if( glewInit() != GLEW_OK )
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

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

	/* SHADER INIT
	 * Create and compile shaders.
	 */
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");

	/* MODEL MESH */
	Mesh test(vertices, nrOfVertices, indices, nrOfIndices);

	/* VAO, VBO, EBO */
	/* GEN VAO AND BIND
	 * VAO (Vertex Array Object) - big box to hold all model (all triangle)
	 */
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray( VAO );	// Activate the box! Anything we'll do to the others buffers it will bind to this VAO. 

	/* GEN VBO AND BIND AND SEND DATA
	 * VBO (Vertex Buffer Object) - is gonna send the vertex data of object.
	 */
	GLuint VBO;
	glGenBuffers(1, &VBO);
		// Make sure that VBO is put in the specific place inside VAO box. Bc can be more than one buffers inside VAO.
	glBindBuffer( GL_ARRAY_BUFFER, VBO );	// Bind VBO as ARRAY_BUFFER inside VAO box. 
		// Data we'll send to the graphics card. Target is ARRAY_BUFFER, send all vertices (size and pointer to data), STATIC_DRAW (as we'll not change vertices often)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* GEN EBO AND BIND AND SEND DATA
	 * EBO (Element Buffer Object) - is gonna send the indices data of object.
	 */
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO );	// Bind Element Array Buffer (EBO) to specific place inside VAO box.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// Determine indices data to be sent to graphics card.

	/* SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	 * Setting specific vertex attributes (position, color and texture coordinate) to determine in what order they are set inside memory.
	 */
	// Position Attribute	// Get position (location) of that attribute inside core_program.
	// 0 - is location inside vertex shader. So we assume to let graphics card to know order of position coordinates inside big table of vertexes.
	// 3 - is size of single data pack used by location 0 (inside vertex shader). 3 floats.
	// GL_FLOAT - data type, 
	// stride = sizeof(Vertex) - how many bytes to move forwart on table to get into next vertex position
	// offsetof - to store offset from first vertex attribute
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

	/* BIND VAO 0 (unbind previously active one)
	 * Unbind previously active VAO. To make sure no VAO is active now.
	 */
	glBindVertexArray( 0 );

	/* INIT TEXTURE 0 and 1 */
	Texture texture0("Images/atom.png", GL_TEXTURE_2D, GL_TEXTURE0);
	Texture texture1("Images/floor.png", GL_TEXTURE_2D, GL_TEXTURE1);
	 
	/* INIT MATERIAL OBJECTS */
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

	/* INIT MATRICES
	 * Adapt local coordinates to global ones.
	 */
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ModelMatrix(1.f);	//Create identity matrix

		// Initialize all three modifications with zero values.
	ModelMatrix = glm::translate(ModelMatrix, position);	// vec3 - translation vector
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));		// Choose angle of rotation and then rotation axis. (X)
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));		// Choose angle of rotation and then rotation axis. (Y)
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));		// Choose angle of rotation and then rotation axis. (Z)
	ModelMatrix = glm::scale(ModelMatrix, scale);	// vec3 - scale vector ( 1 means = no scaling )

		//Initialize ViewMatrix
	glm::vec3 worldUp(0.f, 1.f, 0.f);	// Camera coordinates.
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

		// Initialize Perspective matrix
	float fov = 90.f;	// Field of view. 90 degrees.
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 ProjectionMatrix(1.f);

	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight), 
		nearPlane, 
		farPlane
	);

		// LIGHTS
	glm::vec3 lightPos0(0.f, 0.f, 1.f);

	   	//Init Uniforms
	core_program.setMat4fv(ModelMatrix, "ModelMatrix");
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPosition");

	/* Main Loop */
	while( !glfwWindowShouldClose(window) )
	{
		/* CHECK INPUT */
		glfwPollEvents();
		updateInput( window, position, rotation, scale);

		/* UPDATE */
		updateInput(window);

		/* DRAW */
			// Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.

		/* ---------------   START OF CURRENT CORE_PROGRAM --------------- */
			// Update uniforms (variables send to gpu [shader] from cpu)- every change they're updated.
		material0.sendToShader(core_program);
		core_program.setMat4fv(ModelMatrix, "ModelMatrix");	// Update uniform ModelMatrix variable

				// Update frame buffers size, and send new Projection Matrix.
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		ProjectionMatrix = glm::perspective
		(
			glm::radians(fov), 
			static_cast<float>(framebufferWidth)/framebufferHeight, 
			nearPlane, 
			farPlane
		);
		
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

			// Move, rotate & scale
		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position);	// vec3 - translation vector
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));		// Choose angle of rotation and then rotation axis. (X)
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));		// Choose angle of rotation and then rotation axis. (Y)
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));		// Choose angle of rotation and then rotation axis. (Z)
		ModelMatrix = glm::scale(ModelMatrix, scale);	// vec3 - scale vector ( 1 means = no scaling )

			// Use a program (shader) - need to tell what shader we want to use.
		core_program.use();

			// Activate Texture
		texture0.bind();
		texture1.bind();

			// Bind Vertex Array Object (VAO) to the selected program (shaders).
		glBindVertexArray( VAO );
		
			// Draw
		//glDrawArrays( GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);	// Draw triangles. made of nrOfIndices which are unsigned int, starting from 0 index.
		test.render( &core_program );

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

	/* End of program */
	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}

/*	----------------------------------------------------------
*	Function name:	framebuffer_resize_callback()
*	Parameters:	GLFWwindow* window - window to be used in resize
*			int framebufferWidth - new frame buffer width
*			int framebufferHeight - new frame buffer height
*	Used to:		Set new size of viewport baser on given width and height values.
*	Return:		none
*/
void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight )
{
	glViewport(0, 0, framebufferWidth, framebufferHeight);
}

/*	----------------------------------------------------------
*	Function name:	loadShaders()
*	Parameters:	GLuint& program - reference to program which shaders need to be linked in.
*	Used to:		Load and compile vertex and fragment shader.
*	Return:		bool - is shaders are properly loaded and linked.
*/
bool loadShaders( GLuint& program )
{

	bool loadSucces = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex Shader
	in_file.open("vertex_core.glsl");

	/* Read all from source file of vertex shader. */
	if( in_file.is_open() )
	{
		while( std::getline( in_file, temp) )
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
		loadSucces = false;
	}
		
	in_file.close();

	/* Create and compile vertex shader */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertex_src = src.c_str();
	glShaderSource( vertexShader, 1, &vertex_src, NULL);
	glCompileShader( vertexShader );

	/* Check for compilation errors */
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success);
	if( !success )
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		
		glGetShaderInfoLog( vertexShader, 512, NULL, infoLog);
		std::cout << infoLog << "\n";

		/* If vertex shader fail to compile- do not try to compile fragment shader */
		return false;
	}

	temp = "";
	src = "";

	/* Fragment */
	in_file.open("fragment_core.glsl");

	/* Read from source of fragment shader */
	if( in_file.is_open() )
	{
		while( std::getline(in_file, temp ) )
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSucces = false;
	}
		
	in_file.close();

	/* Create and compile fragment shader */
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragment_src = src.c_str();
	glShaderSource( fragmentShader, 1, &fragment_src, NULL);
	glCompileShader( fragmentShader );

	/* Check for compilation errors */
	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success);
	if( !success )
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";

		glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog);
		std::cout << infoLog << "\n";

		/* If fragment shader fail to compile- do not create main program.
		 * Previously created vertex shader need to be deleted.
		 */
		glDeleteShader( vertexShader );
		return false;
	}

	/* Create Program */
	program = glCreateProgram();

	/* Attach shaders to newly created program. */
	glAttachShader( program, vertexShader );
	glAttachShader( program, fragmentShader );

	/* Link Attached shaders to the program. */
	glLinkProgram( program );

	/* Check for linking errors */
	glGetProgramiv( program, GL_LINK_STATUS, &success );
	if( !success )
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";

		glGetProgramInfoLog( program, 512, NULL, infoLog);
		std::cout << infoLog << "\n";

		loadSucces = false;
	}

	/* End. Tidy up. Delete Shaders. 
	 * Once, you've created program you do not need these individuals shaders anymore. 
	 * They'll be linked into program. 
	 */
	glUseProgram( 0 );
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	return loadSucces;
}

/*	----------------------------------------------------------
*	Function name:	CreateAndBindTexture()
*	Parameters:	const char* src_path - Path to input image
*	Used to:		Create and bind new texture to GL_TEXTURE_2D socket.
*	Return:		GLuint - newly created texture bound.
*/
GLuint CreateAndBindTexture( const char* src_path )
{
	/* INIT TEXTURE
	 * Load an input image using SOIL library. Generate and bind texture object.
	 */
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image( src_path, &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

	GLuint texture0;
	glGenTextures(1, &texture0);	// Generate empty texture object. 

	glBindTexture(GL_TEXTURE_2D, texture0);	// Bind texture to work on it. 

		//SET TEXTURE PARAMETERS (OPTIONS)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Repeat texture if is enough space to do so. S- is tex coordinates (x coordinate). Same as U and V.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// Y coordinate.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	 // Characteristic while magnifying (zooming). Anti-aliasing for texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Characteristic while minimization. Similar as magnification. 

	if( image )	//Check for errors. If image is loaded correctly - create specific OpenGL texture object. Automatically associated with previously bound 'texture0' variable.
	{
		// Create OpenGL texture using image data stored inside 'image' variable.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		// Mipmap is basically same image in several different resolutions to use it depends of distance of viewing object.
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbound any texture!!! Tidy up purpose. 
	SOIL_free_image_data(image);	// Image is loaded, not need to hold its raw data.
	return texture0;
}