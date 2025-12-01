#pragma once

#include "ITexture.h"

#include <GLM/glm.hpp>
#include <string>
#include <memory>

namespace Luna
{
	struct Material
	{
		glm::vec4 m_AmbientColour;
		double m_AmbientIntensity;

		glm::vec4 m_SpecularColour;
		double m_SpecularIntensity;

		std::string m_Path;

		std::shared_ptr<ITexture> m_Albedo = nullptr;
		std::shared_ptr<ITexture> m_SpecularMap = nullptr;
		std::shared_ptr<ITexture> m_NormalMap = nullptr;
	};
}