#pragma once

#include <vector>
#include <string>

#include "GLM/glm.hpp"

#include "Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec2 TextureCoordiante;
	glm::vec3 Normal;
};

class Mesh {
public:
	std::string name = "Static Mesh";

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void BuildMesh();
	void BindMesh();

private:
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int EBO = -1;
};
