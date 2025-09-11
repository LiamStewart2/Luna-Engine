#pragma once

#include "GLM/glm.hpp"

struct Material
{
	Material(glm::vec3 _color) : color(_color) {}
	glm::vec3 color;
};