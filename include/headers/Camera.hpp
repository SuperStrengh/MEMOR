#pragma once

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position = glm::vec3(-3.0f,0.0f,0.0f);
	glm::vec3 LookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	glm::mat4 Projection = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	float nearPlane; 
	float farPlane;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position, float nearPlane, float farPlane);

	// Updates the camera matrix to the Vertex Shader
	void updateMatrix(float FOVdeg, int w, int h, glm::vec3 pos);
	// Exports the camera matrix to a shader
	// Handles camera inputs

	glm::vec3 GetPosition();
};