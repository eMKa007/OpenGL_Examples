#include "ShadowMapFBO.h"

ShadowMapFBO::ShadowMapFBO()
{
	this->m_fbo = 0;
	this->m_shadowMap = 0;
}

ShadowMapFBO::~ShadowMapFBO()
{
	if( m_fbo ) 
		glDeleteFramebuffers(1, &m_fbo);

    if( m_shadowMap )
        glDeleteTextures(1, &m_shadowMap);
}

void ShadowMapFBO::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_fbo);
}

void ShadowMapFBO::BindForReading(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, this->m_shadowMap);
}

bool ShadowMapFBO::Init(unsigned WindowWidth, unsigned WindowHeight)
{
	glGenFramebuffers(1, &this->m_fbo);

	this->InitDepthMapTexture(WindowWidth, WindowHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);
	glFramebufferTexture2D( GL_FRAMEBUFFER, 
		GL_DEPTH_ATTACHMENT, 
		GL_TEXTURE_2D, 
		this->m_shadowMap, 
		0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		std::cout << "ERROR::DEPTH_FRAME_BUFFER_OBJECT_CREATION::STATUS: " << status << std::endl;
		throw std::exception("ERROR::DEPTH_FRAME_BUFFER_OBJECT_CREATION");
	}

}

void ShadowMapFBO::InitDepthMapTexture(unsigned TextureWidth, unsigned TextureHeight)
{
	glGenTextures(1, &this->m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, this->m_shadowMap);
	glTexImage2D( GL_TEXTURE_2D, 0, 
		GL_DEPTH_COMPONENT, TextureWidth, TextureHeight, 
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}
