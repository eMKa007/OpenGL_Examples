#include "game.h"

bool loadShaders( GLuint& program );
GLuint CreateAndBindTexture( const char* src_path );
void updateInput( GLFWwindow* window )
{
	if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void updateInput( GLFWwindow* window, Mesh& mesh )
{
	if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
	{
		mesh.move(glm::vec3(0.f, 0.f, -0.05f));
	}

	if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
	{
		mesh.move(glm::vec3(-0.05f, 0.f, 0.f));
	}

	if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
	{
		mesh.move(glm::vec3(0.f, 0.f, 0.05f));
	}

	if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS )
	{
		mesh.move(glm::vec3(0.05f, 0.f, 0.f));
	}

	if( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS )
	{
		mesh.rotate(glm::vec3(0.f, -1.f, 0.f));
	}

	if( glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS )
	{
		mesh.rotate(glm::vec3(0.f, 1.f, 0.f));
	}

	if( glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS )
	{
		mesh.scaleUp(glm::vec3(0.1f));
	}

	if( glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS )
	{
		mesh.scaleUp(glm::vec3(-0.1f));
	}
}

GLFWwindow* createWindow(
	const char* title, 
	const int width, const int height,
	int& fbwidth, int& fbheight,
	const int GLmajorVer, const int GLminorVer,
	bool resizable = false );

int main( int argc, char* argv[] )
{
	Game game("Tutorial_001", 
		640,
		480,
		4,
		4,
		false);

	/* Main Loop */
	while( !game.getWindodShouldClose() )
	{
		game.update();
		game.render();
	}

	return 0;
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

GLFWwindow* createWindow(const char* title, const int width, const int height, int& fbwidth, int& fbheight,
	const int GLmajorVer, const int GLminorVer, bool resizable)
{
	//Use OpenGL core profile, OpenGL version 4.4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLmajorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLminorVer);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	GLFWwindow* window = glfwCreateWindow( width, height, title, NULL, NULL);

	// Created frame buffer is the same as window. Created window is not resizeable
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	// Canvas size. From (0,0) to (framebufferWidth, framebufferHeight)
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Switched to resizeable window
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);
		
	// Important!!! Bind created window to thread. !!!
	glfwMakeContextCurrent(window);

	return window;
}
