#include "Texture.h"

Texture::Texture()
{
	ID = -1;
	width = 0; height = 0; channels = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::BuildTexture(unsigned char* data)
{

}