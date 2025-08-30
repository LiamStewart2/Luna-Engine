#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class Texture
{
public:
	Texture();
	~Texture();

	void BindTexture(Shader* shader);
	void BuildTexture(unsigned char* data);

	int width, height, channels;
	std::string path;

	unsigned int ID;
};