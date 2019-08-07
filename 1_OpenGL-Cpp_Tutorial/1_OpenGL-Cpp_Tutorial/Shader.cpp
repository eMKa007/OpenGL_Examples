#include  "Shader.h"


Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
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

Shader::~Shader()
{
	glDeleteProgram(this->id);
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::use()
{
	glUseProgram(this->id);
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::unuse()
{
	glUseProgram(0);
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::setVec3f(glm::fvec3 value, const GLchar* name)
{
	this->use();

	glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::setVec2f(glm::fvec2 value, const GLchar* name)
{
	this->use();

	glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::setVec1f(GLfloat value, GLchar* name)
{
	this->use();

	glUniform1fv(glGetUniformLocation(this->id, name), 1, &value);

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
{
	this->use();

	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
{
	this->use();

	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::set1i(GLint value, const GLchar* name)
{
	this->use();

	glUniform1i(glGetUniformLocation(this->id, name), value);

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
*/
void Shader::setVec4f(glm::fvec4 value, const GLchar* name)
{
	this->use();

	glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

/*	----------------------------------------------------------
*	Function name:
*	Parameters:	
*	Used to:	
*	Return:	
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
*	Parameters:
*	Used to:
*	Return:
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
