#pragma once

#include <GLM/glm.hpp>

namespace Luna
{
	struct Material
	{
		glm::vec4 m_AmbientColour;
		double m_AmbientIntensity;

		glm::vec4 m_SpecularColour;
		double m_SpecularIntensity;
	};
}