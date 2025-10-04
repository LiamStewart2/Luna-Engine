#pragma once

#include "../../Renderer-2/RendererContext.h"

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

		void Init(const float& viewport_w, const float& viewport_h) override;
		void SwapBuffers() override;

		ID3D11Device* GetDevice() const { return m_Device; }
		ID3D11DeviceContext* GetImmediateContext() const { return m_ImmediateContext; }
		ID3D11RenderTargetView* GetRenderTargetView() const { return m_FrameBufferView; }
		D3D11_VIEWPORT* GetViewport() const { return m_Viewport; }
		ID3D11Texture2D* GetDepthStencil() const { return _depthStencil; }
		ID3D11DepthStencilView* GetDepthStencilView() const { return _depthStencilView; }

	private:
		GLFWwindow* m_WindowHandle;

		void CreateD3DDevice();
		void CreateSwapChainAndFrameBuffer();
		void InitViewport(const float& viewport_w, const float& viewport_h);

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_ImmediateContext = nullptr;
		IDXGIDevice* m_DxgiDevice = nullptr;
		IDXGIFactory2* m_DxgiFactory = nullptr;
		ID3D11RenderTargetView* m_FrameBufferView = nullptr;
		ID3D11Texture2D* _depthStencil = nullptr;
		ID3D11DepthStencilView* _depthStencilView = nullptr;
		IDXGISwapChain1* m_SwapChain = nullptr;
		D3D11_VIEWPORT* m_Viewport = nullptr;
	};
}
