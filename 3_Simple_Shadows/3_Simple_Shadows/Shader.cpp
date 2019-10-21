#include  "Shader.h"

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: const int versionMajor - desired major OpenGL version 
*			const int versionMinor - desired minor OpenGL version 
*			const char* vertexFile - pointer to string consisting path to vertex shader source code
*			const char* fragmentFile - pointer to string consisting path to vertex shader source code
*			const char* geometryFile - pointer to string consisting path to vertex shader source code ( default value is "", not used )
*/
Shader::Shader(const int versionMajor, const int versionMinor, 
				const char* vertexFile, const char* fragmentFile, const char* geometryFile) 
				: versionMajor(versionMajor), versionMinor(versionMinor)
{
	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;

	vertexShader = loadShader( GL_VERTEX_SHADER, vertexFile );

	if( geometryFile != "" )
	{
		geometryShader = loadShader( GL_GEOMETRY_SHADER, geometryFile );
	}

	fragmentShader = loadShader( GL_FRAGMENT_SHADER, fragmentFile );

	this->linkProgram( vertexShader, geometryShader, fragmentShader );

	/* End. Tidy up. Delete Shaders. 
	 * Once, you've created program you do not need these individuals shaders anymore. 
	 * They'll be linked into program. 
	 */
	glDeleteShader( vertexShader );
	glDeleteShader( geometryShader );
	glDeleteShader( fragmentShader );
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created program instance
*/
Shader::~Shader()
{
	glDeleteProgram(this->id);
}

/*	----------------------------------------------------------
*	Function name: use()
*	Parameters:	none
*	Used to: Bind current program to be in use
*	Return:	void
*/
void Shader::use()
{
	glUseProgram(this->id);
}

/*	----------------------------------------------------------
*	Function name: unuse()
*	Parameters:	none
*	Used to: Unbound current program from scope
*	Return:	void
*/
void Shader::unuse()
{
	glUseProgram(0);
}

/*	----------------------------------------------------------
*	Function name: setVec4f()
*	Parameters:	glm::fvec4 value - vector of four floats to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*	Used to: Pass vector data to shader by given variable name
*	Return:	void
*/
void Shader::setVec4f(glm::fvec4 value, const GLchar* name)
{
	this->use();

	glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name: setVec3f()
*	Parameters:	glm::fvec3 value - vector of three floats to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*	Used to: Pass vector data to shader by given variable name
*	Return:	void
*/
void Shader::setVec3f(glm::fvec3 value, const GLchar* name)
{
	this->use();

	glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name: setVec2f()
*	Parameters:	glm::fvec2 value - vector of two floats to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*	Used to: Pass vector data to shader by given variable name
*	Return:	void
*/
void Shader::setVec2f(glm::fvec2 value, const GLchar* name)
{
	this->use();

	glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	GLfloat value - float variable to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*	Used to: Pass float variable to shader by given variable name
*	Return:	void
*/
void Shader::setVec1f(GLfloat value, GLchar* name)
{
	this->use();

	glUniform1fv(glGetUniformLocation(this->id, name), 1, &value);

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name: setMat4fv()
*	Parameters:	glm::mat4 value - 4x4 matrix to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*			GLboolean transpose - logic value, if true then matrix is assumed in row major order. Default parameter = GL_FALSE
*	Used to: Pass matrix data to shader by given variable name
*	Return:	void
*/
void Shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
{
	this->use();

	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name: setMat3fv()
*	Parameters:	glm::mat3 value - 3x3 matrix to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*			GLboolean transpose - logic value, if true then matrix is assumed in row major order. Default parameter = GL_FALSE
*	Used to: Pass matrix data to shader by given variable name
*	Return:	void
*/
void Shader::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
{
	this->use();

	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name: set1i()
*	Parameters:	GLint value - integer value to be passed to shader
*			const GLchar* name - name of uniform's location in shader
*	Used to: Pass integer data to shader by given variable name
*	Return:	void
*/
void Shader::set1i(GLint value, const GLchar* name)
{
	this->use();

	glUniform1i(glGetUniformLocation(this->id, name), value);

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name: loadShaderSource()
*	Parameters:	const char* fileName - Pointer to string which consist input file name
*	Used to: Read input file to temporary buffer and return it if read is finished
*	Return:	std::string
*/
std::string Shader::loadShaderSource(const char* fileName)
{
	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex Shader
	in_file.open(fileName);

	/* Read all from source file of vertex shader. */
	if( in_file.is_open() )
	{
		while( std::getline( in_file, temp) )
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE:  " << fileName << "\n";
	}

	std::string versionNr = 
		std::to_string(this->versionMajor) + 
		std::to_string(this->versionMinor) + 
		"0";

	src.replace(src.find("#version"), 12, "#version " + versionNr);
	in_file.close();

	return src;
}

/*	----------------------------------------------------------
*	Function name: loadShader()
*	Parameters:	GLenum type - type of shader to create
*			char* fileName - path to shader source code
*	Used to: Create a shader of defined type and return it. 
*	Return: GLuint
*/
GLuint Shader::loadShader(GLenum type, const char* fileName)
{
	char infoLog[512];
	GLint success;

	/* Create and compile vertex shader */
	GLuint shader = glCreateShader(type);

	std::string str_src = this->loadShaderSource(fileName).c_str();
	const GLchar* src = str_src.c_str();

	glShaderSource( shader, 1, &src, NULL);
	glCompileShader( shader );

	/* Check for compilation errors */
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success);
	if( !success )
	{
		std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER" << fileName << "\n";
		
		glGetShaderInfoLog( shader, 512, NULL, infoLog);
		std::cout << infoLog << "\n";
	}

	return shader;
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters: GLuint vertexShader - binding to created vertex shader
*			GLuint geometryShader - binding to created geometry shader ( could be not specified )
*			GLuint fragmentShader - binding to created fragment shader
*	Used to: Link Attached shaders to the program.
*	Return: void 
*/
void Shader::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
{
	char infoLog[512];
	GLint success;

	this->id = glCreateProgram();
	glUseProgram(this->id);

	/* Attach vertex shader to created program */
	glAttachShader(this->id, vertexShader);

	/* Attach geometry shader to created program - only if geometry shader is created */
	if(geometryShader)
	{
		glAttachShader(this->id, geometryShader);
	}

	/* Attach fragment shader to created program */
	glAttachShader(this->id, fragmentShader);

	/* Link Attached shaders to the program. */
	glLinkProgram(this->id);

	/* Check for linking errors */
	glGetProgramiv( this->id, GL_LINK_STATUS, &success );
	if( !success )
	{
		std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";

		glGetProgramInfoLog( this->id, 512, NULL, infoLog);
		std::cout << infoLog << "\n";
	}

	glUseProgram( 0 );
}
