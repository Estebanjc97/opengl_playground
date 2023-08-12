#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f);
	worldUp = glm::vec3(0.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = 0.0f;
	turnSpeed = 0.0f;
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W])
	{
		position += front * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S])
	{
		position -= front * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A])
	{
		position -= right * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D])
	{
		position += right * movementSpeed * deltaTime;
	}
}

void Camera::mouseControl(bool* mouseButtons, GLfloat xChange, GLfloat yChange)
{
	if (mouseButtons[GLFW_MOUSE_BUTTON_1])
	{
		xChange *= turnSpeed;
		yChange *= turnSpeed;

		yaw += xChange;
		pitch += yChange;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		Update();
	}
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp)); 
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}