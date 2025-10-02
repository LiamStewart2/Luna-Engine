#pragma once

#include "RendererAPI.h"
#include "RendererContext.h"

namespace Luna
{
	class ReworkedRenderer
	{
	public:
		static void Init(std::shared_ptr<RendererContext> renderContext);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static void Render();

	private:
		static std::shared_ptr<IRendererAPI> s_RendererAPI;
	};
}

