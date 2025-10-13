#pragma once

#include <GLM/glm.hpp>

namespace Luna
{
	struct Material
	{
		glm::vec3 m_AmbientColour;
		double m_AmbientIntensity;

		glm::vec3 m_SpecularColour;
		double m_SpecularIntensity;
	};
}