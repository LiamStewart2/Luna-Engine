#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader() {}
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void BindShader();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, glm::mat4 value) const;
private:
	unsigned int ID = -1;
};