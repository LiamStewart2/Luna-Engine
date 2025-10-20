
#include "IShader.h"

#include <iostream>
#include "RendererAPI.h"

#include "../Platform/DirectX11/DX11Shader.h"

namespace Luna
{
	std::shared_ptr<IShader> IShader::Create(const std::string& filepath)
	{
		switch(IRendererAPI::GetAPI())
		{
		case RendererAPIType::DirectX11:	return std::make_shared<DX11Shader>(filepath);
		case RendererAPIType::OpenGL:		std::cerr << "RendererAPI::OpenGL is currently not supported!\n"; return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n";	return nullptr;
	}
}