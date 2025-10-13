#pragma once

#include <GLM/vec3.hpp>

namespace Luna
{ 
	enum LightType
	{
		Directional,
		Point,
		Spot
	};
	struct Light
	{
		LightType m_Type = LightType::Directional;
		glm::vec3 m_LightColour = glm::vec3(1);
	};
}