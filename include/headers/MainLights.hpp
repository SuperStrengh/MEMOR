#include "Vertex.hpp"

std::vector <glm::vec3> Lights;

void SetLights(glm::vec3 Position)
{
	Lights.push_back(Position);
}

std::vector <glm::vec3> GetLights()
{
	return Lights;
}

void ClearLights()
{
	Lights.clear();
}