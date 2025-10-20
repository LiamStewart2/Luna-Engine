#include "DX11Texture.h"
#include "DX11RendererContext.h"

#include <d3d11_4.h>

namespace Luna
{
	DX11Texture::DX11Texture(const TexturePacket& textureData)
	{
		SetData(textureData);
		BuildTexture();
	}

	DX11Texture::~DX11Texture()
	{
		Release();
	}

	void DX11Texture::BuildTexture()
	{

		D3D11_TEXTURE2D_DESC textureDescription = {};
		textureDescription.Width = m_TextureData.width;
		textureDescription.Height = m_TextureData.height;
		textureDescription.MipLevels = 0;
		textureDescription.ArraySize = 1;
		textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDescription.SampleDesc.Count = 1;
		textureDescription.Usage = D3D11_USAGE_DEFAULT;
		textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDescription.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		textureDescription.CPUAccessFlags = 0;

		ID3D11Device* device = DX11RendererContext::GetContext()->GetDevice();
		ID3D11DeviceContext* context = DX11RendererContext::GetContext()->GetImmediateContext();

		if (!m_TextureData.buffer)
		{
			std::cerr << "Texture buffer is null" << std::endl;
			return;
		}
		HRESULT hr = device->CreateTexture2D(&textureDescription, nullptr, &m_Texture);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create texture resource" << std::endl;
			return;
		}

		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDescription.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		hr = device->CreateShaderResourceView(m_Texture, &srvDesc, &m_TextureView);
		if (FAILED(hr))
		{
			m_Texture->Release();
			std::cerr << "Failed to create texture resource view" << std::endl;
			return;
		}

		context->UpdateSubresource(m_Texture, 0, nullptr, m_TextureData.buffer, m_TextureData.width * 4, 0);
		context->GenerateMips(m_TextureView);

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = device->CreateSamplerState(&samplerDesc, &m_SamplerState);
		if (FAILED(hr))
		{
			m_Texture->Release();
			std::cerr << "Failed to create texture resource view" << std::endl;
			return;
		}
	}

	void DX11Texture::BindTexture(unsigned int slot) const
	{
		DX11RendererContext::GetContext()->GetImmediateContext()->PSSetShaderResources(slot, 1, &m_TextureView);
		DX11RendererContext::GetContext()->GetImmediateContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void* DX11Texture::GetTextureReference()
	{
		return m_TextureView;
	}

	void DX11Texture::Release()
	{
		if(m_SamplerState) m_SamplerState->Release();
		if(m_TextureView) m_TextureView->Release();
		if(m_Texture) m_Texture->Release();
	}
}