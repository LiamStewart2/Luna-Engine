#pragma once

#include "IFramebuffer.h"
#include "../Core/SceneManager.h"
#include "RendererAPI.h"

namespace Luna
{
	class ShadowRenderer
	{
	public:
		ShadowRenderer();
		~ShadowRenderer();

		static void InitFramebuffer();
		static void ClearFramebuffer();
		// Renders the depth map buffer of one light
		static void ShadowPass(std::shared_ptr<IRendererAPI> rendererAPI, SceneManager* sceneManager, ObjectTransformPairing<Camera>* camera);
		static std::shared_ptr<IFramebuffer> GetLightFramebuffer() { return s_LightFramebuffer; }
		
	private:
		static std::shared_ptr<IFramebuffer> s_LightFramebuffer;
	};
}
