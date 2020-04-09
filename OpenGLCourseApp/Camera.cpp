#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;

	update();
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

void Camera::KeyControl(bool* keys)
{
	// Movement Controls
	if (keys[GLFW_KEY_W]) {
		position += front * movementSpeed;
	}
	
	if (keys[GLFW_KEY_S]) {
		position -= front * movementSpeed;
	}
	
	if (keys[GLFW_KEY_A]) {
		position -= right * movementSpeed;
	}
	
	if (keys[GLFW_KEY_D]) {
		position += right * movementSpeed;
	}
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);	// Returns a View matrix 
}

Camera::~Camera()
{

}