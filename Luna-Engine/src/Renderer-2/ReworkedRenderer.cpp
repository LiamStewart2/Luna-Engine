#include "ReworkedRenderer.h"

namespace Luna
{
	std::shared_ptr<IRendererAPI> ReworkedRenderer::s_RendererAPI = IRendererAPI::Create();

	void ReworkedRenderer::Init(std::shared_ptr<RendererContext> renderContext)
	{
		s_RendererAPI->Init(renderContext);
	}
	void ReworkedRenderer::Shutdown()
	{
	}
	void ReworkedRenderer::BeginFrame()
	{
		s_RendererAPI->StartFrame();
	}
	void ReworkedRenderer::EndFrame()
	{
		s_RendererAPI->EndFrame();
	}
	void ReworkedRenderer::Render()
	{
		// BIND EVERYTHING HERE

		s_RendererAPI->RenderIndexed(0);
	}
}