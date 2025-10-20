#pragma once

#include "../../Renderer-2/IMesh.h"

struct ID3D11Buffer;

namespace Luna
{
	class DX11Mesh : public IMesh
	{
	public:
		DX11Mesh(const std::vector<Vertex>& vertices = {}, const std::vector<unsigned int>& indices = {});
		virtual ~DX11Mesh();

		virtual void BuildMesh() override;
		virtual void BindMesh() override;
	private:
		void Release();

		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
	};
}