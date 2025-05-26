#pragma once

#include <vector>

#include "Vertex.h"
#include "Texture.h"

struct Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};