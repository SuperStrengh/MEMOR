#pragma once

#include "Model.hpp"
#include "Camera.hpp"

glm::vec3 skyboxVertices[] =
{
	//   Coordinates
	glm::vec3(-1.0f, -1.0f,  1.0f),//        7--------6
	glm::vec3( 1.0f, -1.0f,  1.0f),//       /|       /|
	glm::vec3( 1.0f, -1.0f, -1.0f),//      4--------5 |
	glm::vec3(-1.0f, -1.0f, -1.0f),//      | |      | |
	glm::vec3(-1.0f,  1.0f,  1.0f),//      | 3------|-2
	glm::vec3( 1.0f,  1.0f,  1.0f),//      |/       |/
	glm::vec3( 1.0f,  1.0f, -1.0f),//      0--------1
	glm::vec3(-1.0f,  1.0f, -1.0f)
};

unsigned int skyboxIndices[] =
{
	// Right
	5, 1, 2, 6,
	// Left
	3, 0, 4, 7,
	// Top
	7, 4, 5, 6,
	// Bottom
	1, 0, 3, 2,
	// Back
	4, 0, 1, 5,
	// Front
	2, 3, 7, 6,
};

std::vector <Texture> ID;

std::string facesCubemap[6] =
{
    "res/gfx/skybox/skyboxPartUpsideDown.jpg",
    "res/gfx/skybox/skyboxPart.png",
    "res/gfx/skybox/skyboxPart2.png",
    "res/gfx/skybox/skyboxPart2Bottom.jpg",
    "res/gfx/skybox/skyboxPart.png",
    "res/gfx/skybox/skyboxPart.png"
};

void SetupSkyboxTextures()
{
    for (unsigned int i = 0; i < 6; i++)
	{
		ID.push_back(Texture(facesCubemap[i]));
	}
}

void DrawSkybox(glm::mat4 matrix = glm::mat4(1.0f))
{
    unsigned int j = 0;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glLoadMatrixf(glm::value_ptr(matrix));

    for (unsigned int i = 0; i < 24; i)
	{
        ID[j].Bind();

        glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(skyboxVertices[skyboxIndices[i]].x, skyboxVertices[skyboxIndices[i]].y, skyboxVertices[skyboxIndices[i]].z);

        i++;

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(skyboxVertices[skyboxIndices[i]].x, skyboxVertices[skyboxIndices[i]].y, skyboxVertices[skyboxIndices[i]].z);

        i++;

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(skyboxVertices[skyboxIndices[i]].x, skyboxVertices[skyboxIndices[i]].y, skyboxVertices[skyboxIndices[i]].z);

        i++;
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(skyboxVertices[skyboxIndices[i]].x, skyboxVertices[skyboxIndices[i]].y, skyboxVertices[skyboxIndices[i]].z);

        i++;

        glEnd();

        ID[j].Unbind();


        j++;
    }

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}