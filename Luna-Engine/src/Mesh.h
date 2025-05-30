#pragma once

#include <vector>
#include <string>

#include "Vertex.h"
#include "Texture.h"

struct Mesh {
public:
	std::string name = "Static Mesh";
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};