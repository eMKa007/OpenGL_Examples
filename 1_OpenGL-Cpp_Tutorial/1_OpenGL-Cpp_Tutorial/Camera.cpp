#include "Camera.h"


void Camera::updateCameraVectors()
{

}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
{
	this->ViewMatrix = glm::mat4(1.f);

	this->movementSpeed = 3.f;
	this->sensitivity = 5.f;

	this->worldUp = worldUp;
	this->position = position;
	this->right = glm::vec3(0.f);
	this->up = worldUp;

	this->pitch = 0.f;
	this->yaw = -90.f;
	this->roll = 0.f;

	this->updateCameraVectors();
}


Camera::~Camera()
{
}

void Camera::updateKeyboardInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	// Update Position vector
	switch (direction)
	{
	case FORWARD:
		this->position += this->front * this->movementSpeed * dt;
		break;

	case BACKWARD:
		this->position -= this->front * this->movementSpeed * dt;
		break;

	case LEFT:
		this->position -= this->right * this->movementSpeed * dt;
		break;

	case RIGTH:
		this->position += this->right * this->movementSpeed * dt;
		break;

	default:
		break;
	}
}

void Camera::updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
	// Update Pitch, Yaw and Roll
	this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity * dt;
	this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt;

	if(this->pitch >= 80.f )
		this->pitch = 80.f;
	else if ( this->pitch < -80.f )
		this->pitch = -80.f;

	if (this->yaw > 360.f || this->yaw < -360.f)
		this->yaw = 0.f;
}

void Camera::updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	this->updateMouseInput(dt, offsetX, offsetY);
	this->updateKeyboardInput(dt, direction, offsetX, offsetY);
}
