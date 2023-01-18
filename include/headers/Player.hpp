#pragma once

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include<iostream>

#include "Camera.hpp"
#include "Audio.hpp"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <vector>


glm::vec3 PlayerInputs(float speed, glm::vec3 Position);

void SetSprite(float fps, glm::vec3 InputsPosition, glm::vec3 Position);

std::string GetSprite();

glm::vec3 GetPos();
void SetPos(glm::vec3 pos);

float GetPlayerY();

//Get the Key Status
void SetKeyStatus(bool RightKeyPressed, bool LeftKeyPressed, bool UpKeyPressed, bool DownKeyPressed);