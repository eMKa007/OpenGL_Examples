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
	GLint textureUnit;

public:
	/* ------ CONSTRUCTORS/DESTRUCTORS ------ */
	Texture(const char* fileName, GLenum type,  GLint texture_unit);
	~Texture();

	GLuint getID() const { return this->id; }
	GLint getTextureUnit() const { return this->textureUnit-GL_TEXTURE0; }
	void bind();
	void unbind();
	void loadFromFile( const char* fileName );
};

