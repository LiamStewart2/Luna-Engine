#include "RendererAPI.h"

#include <iostream>

#include "../Platform/DirectX11/DX11RendererAPI.h"

namespace Luna
{
	RendererAPIType IRendererAPI::s_API = RendererAPIType::DirectX11;

	std::shared_ptr<IRendererAPI> Create()
	{
		switch (IRendererAPI::GetAPI())
		{
			case RendererAPIType::None:			std::cerr << "RendererAPI::None is currently not supported!\n"; return nullptr;
			case RendererAPIType::DirectX11:    return std::make_shared<DX11RendererAPI>();
			case RendererAPIType::OpenGL:		std::cerr << "RendererAPI::OpenGL is currently not supported!\n"; return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n"; return nullptr;
	}
}