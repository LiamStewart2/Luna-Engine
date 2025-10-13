#include "IMesh.h"

#include <iostream>
#include "RendererAPI.h"

#include "../Platform/DirectX11/DX11Mesh.h"

namespace Luna 
{
	std::shared_ptr<IMesh> IMesh::Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		switch (IRendererAPI::GetAPI())
		{
			case RendererAPIType::DirectX11:	return std::make_shared<DX11Mesh>(vertices, indices);
			case RendererAPIType::OpenGL:		return nullptr;
		}
		std::cerr << "Unknown RendererAPI!\n"; return nullptr;
	}
}