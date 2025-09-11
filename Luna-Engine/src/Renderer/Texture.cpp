#include "Texture.h"

Texture::Texture()
{
	ID = -1;
	width = 0; height = 0; channels = 0;
}

Texture::~Texture()
{

}

void Texture::BindTexture(Shader* shader)
{
	shader->BindShader();
	glActiveTexture(GL_TEXTURE0);
	shader->SetInt("diffuseTexture", 0);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::BuildTexture(unsigned char* data)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	if(width == height)
		glGenerateMipmap(GL_TEXTURE_2D);
}