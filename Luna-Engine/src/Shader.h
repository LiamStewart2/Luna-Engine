#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	Shader() {}
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void BindTexture();

	void SetBool(const std::string name, bool value);
	void SetInt(const std::string name, int value);
	void SetFloat(const std::string name, float value);
private:
	unsigned int ID = -1;
};