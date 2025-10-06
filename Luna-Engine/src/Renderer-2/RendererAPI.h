#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "RendererContext.h"

namespace Luna
{ 
	enum RendererAPIType
	{
		DirectX11,
		OpenGL
	};

	class IRendererAPI
	{
	public:
		virtual ~IRendererAPI() = default;

		virtual void Init(std::shared_ptr<RendererContext> renderContext) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void RenderIndexed(unsigned int count) = 0;

		static RendererAPIType GetAPI() { return s_API; }
		static std::shared_ptr<IRendererAPI> Create();

	private:
		static RendererAPIType s_API;

		std::shared_ptr<RendererContext> m_RenderContext;
	};
}