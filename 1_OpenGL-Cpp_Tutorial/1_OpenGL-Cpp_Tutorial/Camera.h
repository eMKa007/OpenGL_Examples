#pragma once

#include <iostream>

/* OpenGL Extension Wrangler */
#include <glew.h>

#include <glfw3.h>

/* GLM - OpenGL Mathematics */
#include <glm.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	/* FUNCTIONS */
	void updateCameraVectors();

public:
	Camera( glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp );
	virtual ~Camera();
	
	/* ACCESSORS */
	const glm::mat4 getViewMatrix()	{
		this->updateCameraVectors();
		glm::lookAt( this->position, this->position + this->front, this->worldUp);
		return this->ViewMatrix;
	}

	const glm::vec3 getPosition() { return this->position; }

	/* FUNCTIONS */
	void updateKeyboardInput( const float& dt, const int direction, const double& offsetX, const double& offsetY);
	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY); 
	void updateInput( const float& dt, const int direction, const double& offsetX, const double& offsetY);
};

