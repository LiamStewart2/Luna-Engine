#pragma once

#include <memory>

namespace Luna
{
	class RendererContext
	{
	public:
		virtual ~RendererContext() = default;

		virtual void Init(const float& viewport_w, const float& viewport_h) = 0;
		virtual void SwapBuffers() = 0;

		static std::shared_ptr<RendererContext> Create(void* windowHandle);
	};
}
