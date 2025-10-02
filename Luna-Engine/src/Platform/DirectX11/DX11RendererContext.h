#pragma once

#include "../../Renderer-2/RendererContext.h"

struct GLFWwindow;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIDevice;
struct IDXGIFactory2;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;

namespace Luna
{
	class DX11RendererContext : public RendererContext
	{
	public:
		DX11RendererContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

		ID3D11Device* GetDevice() const { return m_Device; }
		ID3D11DeviceContext* GetImmediateContext() const { return m_ImmediateContext; }
	private:
		GLFWwindow* m_WindowHandle;

		void CreateD3DDevice();
		void CreateSwapChainAndFrameBuffer();

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_ImmediateContext = nullptr;
		IDXGIDevice* m_DxgiDevice = nullptr;
		IDXGIFactory2* m_DxgiFactory = nullptr;
		ID3D11RenderTargetView* m_FrameBufferView = nullptr;
		IDXGISwapChain1* m_SwapChain = nullptr;
	};
}
