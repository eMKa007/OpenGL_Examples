#include "Camera.h"


void Camera::updateCameraVectors()
{

}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
{
}


Camera::~Camera()
{
}

void Camera::updateKeyboardInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	// Update Position vector
}

void Camera::updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
	// Update Pitch, Yaw and Roll
}

void Camera::updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	this->updateMouseInput(dt, offsetX, offsetY);
	this->updateKeyboardInput(dt, direction, offsetX, offsetY);
}
