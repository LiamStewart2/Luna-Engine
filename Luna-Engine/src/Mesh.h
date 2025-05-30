#pragma once

#include <vector>
#include <string>

#include "GLM/glm.hpp"

#include "Texture.h"

struct Vertex {
	int positionIndex;
	int textureCoordinateIndex;
	int normalIndex;
};

struct Face {
	std::vector<Vertex> vertices;
};

struct Mesh {
	std::string name = "Static Mesh";

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec2> vertexTextureCoords;
	std::vector<glm::vec3> vertexNormals;

	std::vector<Face> faces;
};
