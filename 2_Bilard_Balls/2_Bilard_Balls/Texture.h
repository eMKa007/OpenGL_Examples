#pragma once

#include <iostream>
#include <string>

#include <glew.h>
#include <glfw3.h>

/* SOIL2 - Simple OpenGL Image Library */
#include <SOIL2.h>

class Texture
{
private:
	/* ------ MEMBER VARIABLES ------ */
	GLuint id;
	int width;
	int height;
	unsigned int type;

public:
	/* ------ CONSTRUCTORS/DESTRUCTORS ------ */
	Texture(const char* fileName, GLenum type);
	~Texture();

	GLuint getID() const { return this->id; }
	void bind(const GLint texture_unit);
	void unbind();
	void loadFromFile( const char* fileName );
};

