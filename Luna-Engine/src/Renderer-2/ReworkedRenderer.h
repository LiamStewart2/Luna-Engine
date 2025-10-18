#pragma once

#include "RendererAPI.h"
#include "RendererContext.h"

#include "../ECS/TransformComponent.h"
#include "../Core/SceneManager.h"
#include "IFramebuffer.h"

namespace Luna
{
	class ReworkedRenderer
	{
	public:
		static void Init(std::shared_ptr<RendererContext> renderContext);
		static void Shutdown();

		static void BeginFrame(SceneManager* sceneManager, IFramebuffer* framebuffer, ObjectTransformPairing<Camera>* camera = nullptr);
		static void EndFrame(SceneManager* sceneManager, IFramebuffer* framebuffer);

		static void Render(SceneManager* sceneManager, IFramebuffer* framebuffer);
	private:
		static std::shared_ptr<IRendererAPI> s_RendererAPI;
	};
}

