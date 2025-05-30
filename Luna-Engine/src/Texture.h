#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	Texture();
	~Texture();

	void BindTexture();
	void BuildTexture(unsigned char* data);

	int width, height, channels;

private:
	unsigned int ID;
};