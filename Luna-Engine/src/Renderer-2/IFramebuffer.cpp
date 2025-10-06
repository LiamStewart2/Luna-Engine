#include "IFramebuffer.h"

#include "RendererAPI.h"
#include <iostream>

#include "../Platform/DirectX11/DX11Framebuffer.h"

namespace Luna
{
	std::shared_ptr<IFramebuffer> IFramebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (IRendererAPI::GetAPI())
		{
			case RendererAPIType::DirectX11:	return std::make_shared<DX11Framebuffer>(spec);
			case RendererAPIType::OpenGL:		std::cerr << "RendererAPI::OpenGL is currently not supported!\n"; return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n"; return nullptr;
	}
}