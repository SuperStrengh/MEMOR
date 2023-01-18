#include "Mesh.hpp"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;
}


void Mesh::Draw
(
	std::string DrawType,
	Camera& camera,
	glm::mat4 matrix,
	float alpha,
	glm::vec3 translation, 
	glm::quat rotation, 
	glm::vec3 scale
)
{


	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	


	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		
		
		textures[i].Bind();
	}

	// Draw the actual mesh
	
	
	glLoadMatrixf(glm::value_ptr(camera.cameraMatrix * matrix));

	

	if(DrawType == "Outlined")
	{
		

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0xFF);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		
		for(int i = 0;i < indices.size(); i)
		{
			glBegin(GL_TRIANGLES);

			//Front face

			glm::vec2 TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;

			TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;

			TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;


		
			glEnd();
		}

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

	}else if(DrawType == "Glow")
	{
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		
		for(int i = 0;i < indices.size(); i)
		{
			glBegin(GL_TRIANGLES);

			//Front face

			glm::vec2 TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;

			TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;

			TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;


		
			glEnd();
		}
	}else{
		

		glColor4f(1.0f, 1.0f, 1.0f, alpha);

		
		
		for(int i = 0;i < indices.size(); i)
		{
			glBegin(GL_TRIANGLES);

			//Front face

			glm::vec2 TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;

			TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;

			TexCoord = glm::mat2(0.0, -1.0, 1.0, 0.0) * glm::vec2(vertices[indices[i]].texUV.x, vertices[indices[i]].texUV.y);

			glNormal3f(vertices[indices[i]].normal.x, vertices[indices[i]].normal.y, vertices[indices[i]].normal.z);
			glTexCoord2f(TexCoord.x, TexCoord.y);
			glVertex3f(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);

			i++;


		
			glEnd();
		}
	}

	glPopMatrix();

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i].Unbind();
	}
}