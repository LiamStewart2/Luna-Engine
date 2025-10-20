#include "DX11Shader.h"

#include <d3d11_4.h>
#include <d3dcompiler.h>

#include "DX11RendererContext.h"

namespace Luna
{
	DX11Shader::DX11Shader(const std::string& filepath)
	{
		m_Path = filepath;
		CompileShader(filepath);
	}

	DX11Shader::~DX11Shader()
	{
		Release();
	}

	void DX11Shader::CompileShader(const std::string& filepath)
	{
		ID3DBlob* errorBlob;
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = S_OK;

		// Vertex Shader
		ID3DBlob* vsBlob;

		std::wstring stemp = std::wstring(filepath.begin(), filepath.end());
		LPCWSTR LFilepath = stemp.c_str();

		hr = D3DCompileFromFile(stemp.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
		if (FAILED(hr))
		{
			std::cerr << "Shader Failed To Compile" << hr << std::endl;
			return;
		}

		hr = DX11RendererContext::GetContext()->GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_VertexShader);

		if (FAILED(hr))
		{
			std::cerr << "Vertex Shader Failed To Compile" << std::endl;
			return;
		}

		// input element creation
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },

		};

		hr = DX11RendererContext::GetContext()->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_InputLayout);
		if (FAILED(hr))
		{
			std::cerr << "Input Layout Failed To Compile" << std::endl;
			return;
		}

		// Pixel Shader
		ID3DBlob* psBlob;

		hr = D3DCompileFromFile(stemp.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
		if (FAILED(hr))
		{
			std::cerr << "line 68 Failed To Compile" << std::endl;
			return;
		}

		hr = DX11RendererContext::GetContext()->GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_PixelShader);
		if (FAILED(hr))
		{
			std::cerr << "Pixel Shader Failed To Compile" << std::endl;
			return;
		}


		vsBlob->Release();
		psBlob->Release();

		DX11RendererContext::GetContext()->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11RendererContext::GetContext()->GetImmediateContext()->IASetInputLayout(m_InputLayout);
	}

	void DX11Shader::Bind()
	{
		DX11RendererContext::GetContext()->GetImmediateContext()->VSSetShader(m_VertexShader, nullptr, 0);
		DX11RendererContext::GetContext()->GetImmediateContext()->PSSetShader(m_PixelShader, nullptr, 0);
	}

	void DX11Shader::Unbind()
	{
		DX11RendererContext::GetContext()->GetImmediateContext()->VSSetShader(nullptr, nullptr, 0);
		DX11RendererContext::GetContext()->GetImmediateContext()->PSSetShader(nullptr, nullptr, 0);
	}
	


	void DX11Shader::Release()
	{
		if (m_VertexShader) m_VertexShader->Release();
		if (m_PixelShader) m_PixelShader->Release();
	}
}