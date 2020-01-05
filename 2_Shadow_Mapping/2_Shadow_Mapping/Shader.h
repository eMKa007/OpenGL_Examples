#pragma once

#include <iostream>
#include <fstream>
#include <string>

/* OpenGL Extension Wrangler */
#include <glew.h>

#include <glfw3.h>

/* GLM - OpenGL Mathematics */
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

/* SOIL2 - Simple OpenGL Image Library */
#include <SOIL2.h>

class Shader
{
private:
	/* ------ MEMBER VARIABLES ------ */
	GLuint id;
	const int versionMajor;
	const int versionMinor;

	/* ------ PRIVATE FUNCTIONS ------ */
	std::string loadShaderSource( const char* fileName );
	GLuint loadShader( GLenum type, const char* fileName );
	void linkProgram( GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);

public:
	/* ------ CONSTRUCTORS/DESTRUCTORS ------ */
	Shader( const int versionMajor, const int versionMinor, const char* vertexFile, const char* fragmentFile, const char* geometryFile = "");
	~Shader();

	/* ------ SET UNIFORMS FUNCTIONS ------ */
	void setVec4f(glm::fvec4 value, const GLchar* name);
	void setVec3f( glm::fvec3 value, const GLchar* name);
	void setVec2f(glm::fvec2 value, const GLchar* name);
	void setVec1f(GLfloat value, GLchar* name);
	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);
	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);
	void set1i(GLint value, const GLchar* name);

	GLuint getID();

	void use();
	void unuse();	
};