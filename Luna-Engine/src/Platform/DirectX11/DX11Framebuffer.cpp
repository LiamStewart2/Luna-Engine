#include "DX11Framebuffer.h"

#include <d3d11_4.h>
#include <Windows.h>

#include "DX11RendererContext.h"

namespace Luna
{
	//Converts the Luna API Texture Formats into DX11 compatible Format types
	static DXGI_FORMAT ToDXGIFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case FramebufferTextureFormat::RGBA16F: return DXGI_FORMAT_R16G16B16A16_FLOAT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}
	static DXGI_FORMAT ToDXGIDepthFormat(DepthTextureFormat format)
	{
		switch (format)
		{
		case DepthTextureFormat::DEPTH24STENCIL8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case DepthTextureFormat::DEPTH32F:        return DXGI_FORMAT_D32_FLOAT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}

	DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification& spec)
		: m_Spec(spec)
	{
		Invalidate();
	}
	DX11Framebuffer::~DX11Framebuffer()
	{
		Release();
	}

	void DX11Framebuffer::Bind()
	{
		Invalidate();
		DX11RendererContext::GetContext()->GetImmediateContext()->OMSetRenderTargets((UINT)m_ColorRTVs.size(), m_ColorRTVs.data(), m_DSV);

		// Update viewport to framebuffer size
		D3D11_VIEWPORT vp{};
		vp.Width = (FLOAT)m_Spec.m_Width;
		vp.Height = (FLOAT)m_Spec.m_Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		DX11RendererContext::GetContext()->GetImmediateContext()->RSSetViewports(1, &vp);
	}

	void DX11Framebuffer::Unbind()
	{
		DX11RendererContext::GetContext()->GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void DX11Framebuffer::Resize(const unsigned int& width, const unsigned int& height)
	{
		m_Spec.m_Width = width; m_Spec.m_Height = height;
		if(!m_Spec.m_SwapChainTarget)
			Invalidate();
	}

	void DX11Framebuffer::Clear(const float color[4])
	{	
		for(size_t i = 0; i < m_Spec.m_ColorAttachments.size(); i++)
			DX11RendererContext::GetContext()->GetImmediateContext()->ClearRenderTargetView(m_ColorRTVs[i], color);
		if(m_DSV)
			DX11RendererContext::GetContext()->GetImmediateContext()->ClearDepthStencilView(m_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	unsigned int DX11Framebuffer::GetWidth() const
	{
		return m_Spec.m_Width;
	}

	unsigned int DX11Framebuffer::GetHeight() const
	{
		return m_Spec.m_Height;
	}
	void* DX11Framebuffer::GetColorAttachment(int index)
	{
		if (index < (int)m_ColorRTVs.size() && index >= 0)
			return m_ColorRTVs[index];
		return nullptr;
	}
	void* DX11Framebuffer::GetDepthAttachment()
	{
		return m_DSV;
	}

	void DX11Framebuffer::Invalidate()
	{
		Release();
		
		ID3D11Device* device = DX11RendererContext::GetContext()->GetDevice();

		// IF IN THE SWAP CHAIN
		// we can access the backbuffer
		if (m_Spec.m_SwapChainTarget)
		{
			ID3D11Texture2D* backbuffer = nullptr;
			DX11RendererContext::GetContext()->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);

			ID3D11RenderTargetView* rtv = nullptr;
			device->CreateRenderTargetView(backbuffer, nullptr, &rtv);
			backbuffer->Release();

			m_ColorRTVs.push_back(rtv);

			DXGI_FORMAT depthForamt = ToDXGIDepthFormat(m_Spec.m_DepthAttachment);
			if (depthForamt != DXGI_FORMAT_UNKNOWN)
			{
				D3D11_TEXTURE2D_DESC backbufferDesc = {};
				backbuffer->GetDesc(&backbufferDesc);

				D3D11_TEXTURE2D_DESC depthDesc = {};
				depthDesc.Width = backbufferDesc.Width;
				depthDesc.Height = backbufferDesc.Width;
				depthDesc.MipLevels = 1;
				depthDesc.ArraySize = 1;
				depthDesc.Format = depthForamt;
				depthDesc.SampleDesc.Count = 1;
				depthDesc.Usage = D3D11_USAGE_DEFAULT;
				depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

				ID3D11Texture2D* depthTex = nullptr;
				device->CreateTexture2D(&depthDesc, nullptr, &depthTex);
				device->CreateDepthStencilView(depthTex, nullptr, &m_DSV);
				depthTex->Release();
			}
			return;
		}


		// IF NOT IN THE SWAP CHAIN
		// then we need to make the textures for DX11
		m_ColorRTVs.resize(m_Spec.m_ColorAttachments.size());
		for (size_t i = 0; i < m_Spec.m_ColorAttachments.size(); i++)
		{
			if(m_Spec.m_ColorAttachments[i] != FramebufferTextureFormat::None)
			{
				const DXGI_FORMAT& colorFormat = ToDXGIFormat(m_Spec.m_ColorAttachments[i]);
				if(colorFormat == DXGI_FORMAT_UNKNOWN) { std::cerr << "Unsupported Framebuffer Format" << std::endl; return; }
				
				
				D3D11_TEXTURE2D_DESC textureDescription = {};
				textureDescription.Width = m_Spec.m_Width; textureDescription.Height = m_Spec.m_Height;
				textureDescription.MipLevels = 1; textureDescription.ArraySize = 1;
				textureDescription.Format = colorFormat;
				textureDescription.SampleDesc.Count = 1;
				textureDescription.Usage = D3D11_USAGE_DEFAULT;
				textureDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

				ID3D11Texture2D* colorTexture = nullptr;
				device->CreateTexture2D(&textureDescription, nullptr, &colorTexture);

				ID3D11RenderTargetView* rtv = nullptr;
				device->CreateRenderTargetView(colorTexture, nullptr, &rtv);
				m_ColorRTVs[i] = rtv;

				colorTexture->Release();
			}
		}


		const DXGI_FORMAT& depthFormat = ToDXGIDepthFormat(m_Spec.m_DepthAttachment);
		if (depthFormat != DXGI_FORMAT_UNKNOWN)
		{	
			CD3D11_TEXTURE2D_DESC depthDescription = {};
			depthDescription.Width = m_Spec.m_Width; depthDescription.Height = m_Spec.m_Height;
			depthDescription.MipLevels = 1;
			depthDescription.ArraySize = 1;
			depthDescription.Format = depthFormat;
			depthDescription.SampleDesc.Count = 1;
			depthDescription.Usage = D3D11_USAGE_DEFAULT;
			depthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			ID3D11Texture2D* depthTex = nullptr;
			device->CreateTexture2D(&depthDescription, nullptr, &depthTex);

			device->CreateDepthStencilView(depthTex, nullptr, &m_DSV);
			depthTex->Release();
		}
		else
		{
			std::cerr << "Unsupported Depth Buffer Format" << std::endl; return;
		}
	}

	void DX11Framebuffer::Release()
	{
		for (auto& rtv : m_ColorRTVs)
		{
			if (rtv)
			{
				rtv->Release();
				rtv = nullptr;
			}
		}
		m_ColorRTVs.clear();

		if (m_DSV)
		{
			m_DSV->Release();
			m_DSV = nullptr;
		}
	}
}