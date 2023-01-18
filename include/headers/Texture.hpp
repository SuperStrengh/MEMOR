#pragma once

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include<iostream>
#include<stb/stb_image.h>
#include<vector>
#include<cmath>

class Texture
{
public:
	GLuint ID;
	const char* type;
	std::string imageName;
	GLuint unit;

	


	Texture(std::string image);

	// Assigns a texture unit to a texture
	void texUnit(const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};