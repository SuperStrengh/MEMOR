#include"Camera.hpp"
#include"math.h"


float distance = 30.0f;

Camera::Camera(int width, int height, glm::vec3 position, float nearPlane, float farPlane)
{
	Camera::width = width;
	Camera::height = height;

	Camera::nearPlane = nearPlane;
	Camera::farPlane = farPlane;

	Position = position;
}

void Camera::updateMatrix(float FOVdeg, int w, int h, glm::vec3 pos)
{
	width = w;
	height = h;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(pos + glm::vec3(0.0f, 30.0f, 45.0f), pos + glm::vec3(0.0f, 5.0f, 0.0f),Up);

	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	Position = pos + glm::vec3(0.0f, 30.0f, 45.0f);

	Projection = projection;

	View = view;

	LookDirection = glm::vec3(0.0f, 25.0f, 45.0f);

	cameraMatrix = projection * view;
}

glm::vec3 Camera::GetPosition()
{
	return Position;
}