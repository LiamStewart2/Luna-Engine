#include "DX11Mesh.h"

#include <d3d11.h>

#include "DX11RendererContext.h"

namespace Luna
{
	DX11Mesh::DX11Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		SetVertexIndexData(vertices, indices);
		BuildMesh();
	}
	DX11Mesh::~DX11Mesh()
	{
		Release();
	}

	void DX11Mesh::BuildMesh()
	{
		Release();

		// Vertex Buffer Initialization
		D3D11_BUFFER_DESC vertexBufferDescription = {};
		vertexBufferDescription.ByteWidth = sizeof(Vertex) * m_VertexData.size();
		vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexData = { m_VertexData.data()};

		DX11RendererContext::GetContext()->GetDevice()->CreateBuffer(&vertexBufferDescription, &vertexData, &m_VertexBuffer);

		// Index Buffer Initialization
		D3D11_BUFFER_DESC indexBufferDescription = {};
		indexBufferDescription.ByteWidth = sizeof(unsigned int) * m_IndexData.size();
		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexData = { m_IndexData.data() };

		DX11RendererContext::GetContext()->GetDevice()->CreateBuffer(&indexBufferDescription, &indexData, &m_IndexBuffer);
	}
	void DX11Mesh::BindMesh()
	{
		UINT stride = { sizeof(Vertex) };
		UINT offset = 0;

		DX11RendererContext::GetContext()->GetImmediateContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		DX11RendererContext::GetContext()->GetImmediateContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11Mesh::Release()
	{
		if(m_VertexBuffer != nullptr)	m_VertexBuffer->Release();
		if(m_IndexBuffer != nullptr)	m_IndexBuffer->Release();
	}
}