#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	Texture();
	~Texture();

	void BindTexture();
private:
	unsigned int ID;
	int width, height, channels;

	void BuildTexture(unsigned char* data);
};