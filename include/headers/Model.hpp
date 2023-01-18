#pragma once

#include<json/json.h>

#include "Mesh.hpp"

using json = nlohmann::json;

class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const char* file = "res/gfx/penguin.gltf",glm::vec3 translation = glm::vec3(0.0f),glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f),glm::vec3 scale = glm::vec3(1.0f),std::string TexturePath = " ", std::string drawType = "Default");

	void Draw(std::string ShaderType, Camera& camera);

	void Rotate(float Rotation = 0.0f, glm::vec3 Axis = glm::vec3(0.0f, 1.0f, 0.0f));
	void Move(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),std::string TexturePath = " ");
	void ChangeTexture(std::string tex);
	void Scale(glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f));

	bool IsNPC = false;
	float Alpha = 1.0f;
	std::string DrawType = "Default";

	glm::vec3 GetPosition()
	{
		return ModelPosition;
	}

	glm::vec3 GetScale()
	{
		return ModelScale;
	}

	glm::quat GetRotation()
	{
		return ModelRotation;
	}

	std::string GetTexture()
	{
		return ModelTexture;
	}

	std::vector<Mesh> GetMeshes()
	{
		return meshes;
	}
private:
	// Variables for easy access
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	// All the meshes and transformations
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Prevents textures from being loaded twice
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;
	std::vector<int> jointsID;
	std::vector<glm::mat4> joints;
	std::vector<float> weights;
	std::vector<float> Ijoints;

	glm::vec3 ModelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::quat ModelRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);

	std::string ModelTexture = "penguin.png";

	// Loads a single mesh by its index
	void loadMesh(unsigned int indMesh,std::string TexturePath);
	void updateMesh(unsigned int indMesh,std::string TexturePath);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f),glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f),glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), std::string TexturePath = " ");
	void UpdatetraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f),glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f),glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), std::string TexturePath = " ");

	// Gets the binary data from a file
	std::vector<unsigned char> getData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> getFloats(json accessor,const unsigned int BufferInd = 0);
	std::vector<int> getInts(json accessor,const unsigned int BufferInd = 0);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures(std::string TexturePath);

	// Assembles all the floats into vertices
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions, 
		std::vector<glm::vec3> normals, 
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

void ClearLightBuffer();

std::vector <glm::vec3> GetLightBuffer();