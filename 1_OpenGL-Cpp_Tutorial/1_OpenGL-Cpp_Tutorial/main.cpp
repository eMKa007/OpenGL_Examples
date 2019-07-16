#include "libs.h"

void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight );
bool loadShaders( GLuint& program );

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
	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	// Canvas size. From (0,0) to (framebufferWidth, framebufferHeight)
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Switched to resizeable window
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
		
	// Important!!! Bind created window to thread.
	glfwMakeContextCurrent(window);

	/* Init GLEW (Needs the window and OpenGL context) */
	glewExperimental = GL_TRUE;

	//Error
	if( glewInit() != GLEW_OK )
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	//SHADER INIT
	GLuint core_program;
	if( !loadShaders(core_program) )
		glfwTerminate();


	/* Main Loop */
	while( !glfwWindowShouldClose(window) )
	{
		/* CHECK INPUT */
		glfwPollEvents();

		/* UPDATE */

		/* DRAW */
			// Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//Clear all three buffers.

			// Draw

			// End Draw
		glfwSwapBuffers(window);
		glFlush();
	}

	/* End of program */
	glfwDestroyWindow( window );
	glfwTerminate();

	/* Delete program */
	glDeleteProgram( core_program );

	

	return 0;
}

/*	----------------------------------------------------------
*	Function name:	framebuffer_resize_callback()
*	Parameters:	GLFWwindow* window - window to be used in resize
*			int framebufferWidth - new framebuffer width
*			int framebufferHeight - new framebuffer height
*	Used to:		Set new size of veiwport baser on given width and height values.
*	Return:		none
*/
void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight )
{
	glViewport(0, 0, framebufferWidth, framebufferHeight);
}

/*	----------------------------------------------------------
*	Function name:	loadShaders()
*	Parameters:	GLuint& program - reference to program which shaderrs need to be linked in.
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