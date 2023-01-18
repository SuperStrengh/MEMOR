#pragma once

#include<fstream>
#include<streambuf>
#include<string>
#include<algorithm>
#include<iterator>
#include<cerrno>
#include<iostream>

#include <glm/gtx/string_cast.hpp>

#include"Camera.hpp"
#include"Texture.hpp"
#include"Vertex.hpp"
#include<json/json.h>

using json = nlohmann::json;

std::string get_file_contents(const char* filename);

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh
	void Draw
	(
		std::string DrawType,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		float alpha = 1.0f,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)


	);

	bool Rendered = false;
};