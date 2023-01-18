#include"Texture.hpp"

Texture::Texture(std::string pixels)
{
	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(pixels.c_str(), &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	if (numColCh == 4)
		glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		widthImg, heightImg,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		bytes);
	else if (numColCh == 3)
		glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		widthImg, heightImg,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bytes);
	else if (numColCh == 1)
		glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		widthImg, heightImg,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		bytes);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);

	imageName = pixels;
}

void Texture::Bind()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID);


	glColor3f(0.9f, 0.95f, 1.0f);
}

void Texture::Unbind()
{
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}