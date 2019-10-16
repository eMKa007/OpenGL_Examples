#pragma once

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

#include "Shader.h"

class Material
{
private:
	/* Colors and intensitives - same as in fragment shader */
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	/* Texture  - same as in fragment shader */
	GLint diffuseTex;
	GLint specularTex;

public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex);
	~Material();

	void sendToShader( Shader& programID );
};

