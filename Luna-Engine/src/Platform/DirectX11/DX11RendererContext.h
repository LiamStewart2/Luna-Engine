#pragma once

#include "../../Renderer-2/RendererContext.h"
#include <iostream>

struct GLFWwindow;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIDevice;
struct IDXGIFactory2;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct D3D11_VIEWPORT;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

namespace Luna
{
	class DX11RendererContext : public RendererContext
	{
	public:
		DX11RendererContext(GLFWwindow* windowHandle);
		~DX11RendererContext();

		static DX11RendererContext* GetContext() 
		{ 
			if(s_Instance != nullptr) return s_Instance; 
			else std::cerr << "No DX11RenderContext Active" << std::endl; 
			return nullptr;
		}

		void Init(const float& viewport_w, const float& viewport_h) override;
		void SwapBuffers() override;

		ID3D11Device* GetDevice() const { return m_Device; }
		ID3D11DeviceContext* GetImmediateContext() const { return m_ImmediateContext; }
		IDXGISwapChain1* GetSwapChain() const {return m_SwapChain; }

	private:
		static DX11RendererContext* s_Instance;

		GLFWwindow* m_WindowHandle;

		void CreateD3DDevice();
		void CreateSwapChainAndFrameBuffer();

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_ImmediateContext = nullptr;
		IDXGIDevice* m_DxgiDevice = nullptr;
		IDXGIFactory2* m_DxgiFactory = nullptr;
		IDXGISwapChain1* m_SwapChain = nullptr;
	};
}
