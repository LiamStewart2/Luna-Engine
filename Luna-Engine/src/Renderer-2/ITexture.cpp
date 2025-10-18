#include "ITexture.h"

#include <iostream>
#include "RendererAPI.h"

#include "../Platform/DirectX11/DX11Texture.h"

namespace Luna
{
	std::shared_ptr<ITexture> ITexture::Create(const TexturePacket& textureData)
	{
		switch (IRendererAPI::GetAPI())
		{
			case RendererAPIType::DirectX11:	return std::make_shared<DX11Texture>(textureData);
			case RendererAPIType::OpenGL:		return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n"; return nullptr;
	}
}