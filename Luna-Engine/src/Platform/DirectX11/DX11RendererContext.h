#pragma once

#include "../../Renderer-2/RendererContext.h"

struct GLFWwindow;

namespace Luna
{
	class DX11RendererContext : public RendererContext
	{
	public:
		DX11RendererContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
