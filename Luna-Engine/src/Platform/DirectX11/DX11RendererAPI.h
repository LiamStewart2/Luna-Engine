#pragma once
#include "../../Renderer-2/RendererAPI.h"
#include "DX11RendererContext.h"

namespace Luna
{
	class DX11RendererAPI : public IRendererAPI
	{
	public:
		virtual ~DX11RendererAPI() = default;

		void Init(std::shared_ptr<RendererContext> renderContext) override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void StartFrame() override;
		void EndFrame() override;

		void RenderIndexed(unsigned int count) override;
	private:
		DX11RendererContext* m_RenderContext = nullptr;
	};
}