#pragma once

#include "IFramebuffer.h"
#include "../Core/SceneManager.h"

namespace Luna
{
	class ShadowRenderer
	{
	public:
		ShadowRenderer();
		~ShadowRenderer();

		// Renders the depth map buffer of one light
		static void ShadowPass(SceneManager* sceneManager);
		static std::shared_ptr<IFramebuffer> GetLightFramebuffer() { return s_LightFramebuffer; }
	private:
		static std::shared_ptr<IFramebuffer> s_LightFramebuffer;
	};
}
