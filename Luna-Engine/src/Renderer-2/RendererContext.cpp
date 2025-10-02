#include "RendererContext.h"

#include <iostream>
#include "RendererAPI.h"

#include "../Platform/DirectX11/DX11RendererContext.h"

namespace Luna
{
	std::shared_ptr<RendererContext> RendererContext::Create(void* WindowHandle)
	{
		switch (IRendererAPI::GetAPI())
		{
			case RendererAPIType::None:			std::cerr << "RendererAPI::None is currently not supported!\n"; return nullptr;
			case RendererAPIType::DirectX11:    return std::make_shared<DX11RendererContext>(static_cast<GLFWwindow*>(WindowHandle));
			case RendererAPIType::OpenGL:		std::cerr << "RendererAPI::OpenGL is currently not supported!\n"; return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n"; return nullptr;
	}

}