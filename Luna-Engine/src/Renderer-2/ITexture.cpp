#include "ITexture.h"

#include <iostream>
#include "RendererAPI.h"

namespace Luna
{
	std::shared_ptr<ITexture> ITexture::Create(const std::string& path)
	{
		switch (IRendererAPI::GetAPI())
		{
			case RendererAPIType::DirectX11:	return nullptr;
			case RendererAPIType::OpenGL:		return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n"; return nullptr;
	}
}