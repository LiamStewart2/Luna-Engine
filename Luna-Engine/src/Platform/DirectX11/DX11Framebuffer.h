#pragma once

#include "../../Renderer/IFramebuffer.h"
#include <vector>

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;

namespace Luna
{
	class DX11Framebuffer : public IFramebuffer
	{
	public:
		DX11Framebuffer(const FramebufferSpecification& spec);
		~DX11Framebuffer();

		void Bind() override;
		void Unbind() override;
		void Resize(const unsigned int& width, const unsigned int& height) override;

		void Clear(const float color[4]) override;

		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		void* GetColorAttachment(int index = 0) override;
		void* GetDepthAttachment() override;
	private:
		void Invalidate();
		void Release();

		FramebufferSpecification m_Spec;
		
		std::vector<ID3D11RenderTargetView*> m_ColorRTVs;
		std::vector<ID3D11ShaderResourceView*> m_ColorSRVs;
		ID3D11DepthStencilView* m_DSV = nullptr;
	
	};
}