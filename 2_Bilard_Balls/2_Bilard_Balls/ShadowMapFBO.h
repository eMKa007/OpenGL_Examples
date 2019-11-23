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

#include <iostream>

class ShadowMapFBO
{
private:
	GLuint m_fbo;	/* Actual Frame Buffer Object */
	GLuint m_shadowMap;

	void InitDepthMapTexture(unsigned TextureWidth, unsigned TextureHeight);

public:
	ShadowMapFBO(unsigned WindowWidth, unsigned WindowHeight);
	~ShadowMapFBO();

	void Init( unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting();

	void BindForReading( GLenum TextureUnit);


};