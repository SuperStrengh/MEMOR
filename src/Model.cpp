#include"Model.hpp"
#include "MainLights.hpp"

Model::Model(const char* file,glm::vec3 translation,glm::quat rotation,glm::vec3 scale,std::string TexturePath, std::string drawType)
{
	// Make a JSON object
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	// Get the binary data
	Model::file = file;
	data = getData();

	DrawType = drawType;

	// Traverse all nodes
	traverseNode(0,glm::mat4(1.0f),translation,rotation, scale, TexturePath);
}

void Model::Rotate(float Rotation, glm::vec3 Axis)
{
	ModelRotation = glm::rotate(glm::mat4(1.0f) , glm::radians(Rotation), Axis);
	
	for (int i = 0; i < matricesMeshes.size(); i++)
	{
		for (int i = 0; i < matricesMeshes.size(); i++)
		{
			matricesMeshes[i] = glm::rotate(matricesMeshes[i] , glm::radians(Rotation), Axis);
		}
	}
}

void Model::Move(glm::vec3 position,std::string TexturePath)
{
	if(TexturePath == " ")
	{
		TexturePath = ModelTexture;
	}

	for (int i = 0; i < matricesMeshes.size(); i++)
	{
		UpdatetraverseNode(0, glm::mat4(1.0f), position, glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), TexturePath);
	}

	ModelPosition = position;
}

void Model::ChangeTexture(std::string TexturePath)
{
	if(TexturePath != ModelTexture)
	{
		for (int i = 0; i < matricesMeshes.size(); i++)
		{
			updateMesh(i, TexturePath);
		}
	}
}

void Model::Scale(glm::vec3 scale)
{
	ModelScale = scale;
	for (int i = 0; i < matricesMeshes.size(); i++)
	{
		matricesMeshes[i] = glm::scale(matricesMeshes[i],scale);
	}
}


void Model::Draw(std::string ShaderType, Camera& camera)
{
	if(DrawType != "Glow")
	{
		std::vector <glm::vec3> lights = GetLightBuffer();

		for(int i = 0; i < lights.size(); i++)
		{
			GLfloat lightColor0[] = {5.0f, 5.0f, 0.0f, 1.0f};
			GLfloat light_attenuation[] = { 0.5f, 0.5f, 0.5f, 1.0f};
			GLfloat lightPos0[] = {lights[i].x - ModelPosition.x, lights[i].y - ModelPosition.x, lights[i].z - ModelPosition.x, 1.0f}; //Positioned at (4, 0, 8)


			glLightfv(GL_LIGHT0+i, GL_POSITION, lightPos0);
			glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lightColor0);
			glLightfv(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, light_attenuation);
		}
	}else{
		SetLights(ModelPosition);
	}
	

	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(ShaderType, camera, matricesMeshes[i], Alpha);
	}	
}

void Model::loadMesh(unsigned int indMesh,std::string TexturePath)
{
	// Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	unsigned int weightAccInd;
	unsigned int jointsAccInd;

	// Use accessor indices to get all vertices components
	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);
	

	// Combine all the vertex components and also get the indices and textures
	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture> textures = getTextures(TexturePath);


	// Combine the vertices, indices, and textures into a mesh
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::updateMesh(unsigned int indMesh,std::string TexturePath)
{

	// Combine the vertices, indices, and textures into a mesh
	meshes[indMesh].textures = getTextures(TexturePath);
}

void ClearLightBuffer()
{	
	ClearLights();
}

std::vector <glm::vec3> GetLightBuffer()
{
	return Lights;
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix,glm::vec3 translation,glm::quat rotation,glm::vec3 scale,std::string TexturePath)
{
	// Current node
	json node = JSON["nodes"][nextNode];

	// Get quaternion if it exists
	if (node.find("rotation") != node.end() && rotation == glm::quat(0.0f, 0.0f, 0.0f, 1.0f))
	{
		
		float rotValues[4] =
		{
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2],
			node["rotation"][3]
		};
		rotation = glm::make_quat(rotValues);
	}

	rotation = glm::quat(rotation.z, rotation.w, rotation.x, rotation.y);
	
	// Get scale if it exists

	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}

	
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}
	
	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(0.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	ModelPosition = translation;
	ModelScale = scale;
	ModelRotation = rotation;
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"], TexturePath);
	}

	if (nextNode + 1 < JSON["nodes"].size())
	{
		traverseNode(nextNode + 1, matNextNode);
	}
}

void Model::UpdatetraverseNode(unsigned int nextNode, glm::mat4 matrix,glm::vec3 translation,glm::quat rotation,glm::vec3 scale,std::string TexturePath)
{
	// Current node
	json node = JSON["nodes"][nextNode];

	// Get scale if it exists

	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}

	
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}
	
	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(0.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	ModelPosition = translation;
	
	sca = glm::scale(sca, ModelScale);
	rot = glm::mat4_cast(ModelRotation);
	trans = glm::translate(trans, translation);
	

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		translationsMeshes[nextNode] = translation;
		matricesMeshes[nextNode] = matNextNode;

		updateMesh(node["mesh"], TexturePath);
	}


	if (nextNode + 1 < JSON["nodes"].size())
	{
		UpdatetraverseNode(nextNode + 1, matNextNode, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), TexturePath);
	}
}

std::vector<unsigned char> Model::getData()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());

	return data;
}

std::vector<float> Model::getFloats(json accessor,const unsigned int BufferInd)
{
	std::vector<float> floatVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else if (type == "MAT4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, MAT4, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	if (accessor["componentType"] == 5121)
	{
		unsigned int beginningOfData = byteOffset + accByteOffset;
		unsigned int lengthOfData = count * 4 * numPerVert;
		for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			//unsigned int value;
			//std::memcpy(&value, bytes, sizeof(unsigned int));
			floatVec.push_back(data[i++]);
		}
	}else{
		unsigned int beginningOfData = byteOffset + accByteOffset;
		unsigned int lengthOfData = count * 4 * numPerVert;
		for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			float value;
			std::memcpy(&value, bytes, sizeof(float));
			floatVec.push_back(value);
		}
	}

	return floatVec;
}

std::vector<int> getInts(json accessor,const unsigned int BufferInd = 0);

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture> Model::getTextures(std::string TexturePath)
{
	std::vector<Texture> textures;

	

	// Go over all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri of current texture
		std::string texPath = JSON["images"][i]["uri"];

		// Check if the texture has already been loaded

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if(TexturePath == " ")
			{
				if (loadedTexName[j] == texPath)
				{
					textures.push_back(loadedTex[j]);
					skip = true;
					break;
				}
			}else{
				if (loadedTexName[j] == TexturePath)
				{
					textures.push_back(loadedTex[j]);

					skip = true;
					break;
				}
			}

			if(skip == true)
			{
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			if(TexturePath == " ")
			{
				std::string fileStr = std::string(file);
				std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

				Texture diffuse = Texture(fileDirectory + texPath);
				
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}else{
				std::string filePath = "res/gfx/" + TexturePath;

				Texture diffuse = Texture(filePath);

				
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(TexturePath);
			}
			
		}
	}

	

	ModelTexture = TexturePath;

	return textures;
}

std::vector<Vertex> Model::assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}

	return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}

	return vectors;
}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	return vectors;
}
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	return vectors;
}