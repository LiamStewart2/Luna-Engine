#include "DX11RendererContext.h"

#include <iostream>
#include <Windows.h>
#include <d3d11_4.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "STB/stb_image.h"

namespace Luna
{
	DX11RendererContext* DX11RendererContext::s_Instance = nullptr;

	DX11RendererContext::DX11RendererContext(GLFWwindow* windowHandle) 
		: m_WindowHandle(windowHandle)
	{
		int width, height;
		stbi_set_flip_vertically_on_load(false);
		glfwGetWindowSize(windowHandle, &width, &height);
		Init((float)width, (float)height);

		if(s_Instance == nullptr)
			s_Instance = this;
		else
			std::cerr << "MULTIPLE DX11RendererContext INSTANCES" << std::endl;
	}

	DX11RendererContext::~DX11RendererContext()
	{
		if (m_SwapChain) m_SwapChain->Release();
		if (m_DxgiFactory) m_DxgiFactory->Release();
		if (m_DxgiDevice) m_DxgiDevice->Release();
		if (m_ImmediateContext) m_ImmediateContext->Release();
		if (m_Device) m_Device->Release();
	}

	void DX11RendererContext::Init(const float& viewport_w, const float& viewport_h)
	{
		CreateD3DDevice();
		CreateSwapChainAndFrameBuffer();
	}

	void DX11RendererContext::SwapBuffers()
	{
		m_SwapChain->Present(0, 0);
	}

	void DX11RendererContext::CreateD3DDevice()
	{
		HRESULT hr = S_OK;

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};

		ID3D11Device* baseDevice;
		ID3D11DeviceContext* baseDeviceContext;

		DWORD createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | createDeviceFlags,
			featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &baseDevice, nullptr, &baseDeviceContext);

		if (FAILED(hr))
		{
			std::cerr << "Failed to create D3D11 device and context! HRESULT: " << hr << std::endl;
			return;
		}

		hr = baseDevice->QueryInterface(__uuidof(ID3D11Device), reinterpret_cast<void**>(&m_Device));
		hr = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext), reinterpret_cast<void**>(&m_ImmediateContext));

		baseDevice->Release();
		baseDeviceContext->Release();

		hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&m_DxgiDevice));
		if (FAILED(hr))
		{
			std::cerr << "Failed to create D3D11 device and context! HRESULT: " << hr << std::endl;
			return;
		}

		IDXGIAdapter* dxgiAdapter;
		hr = m_DxgiDevice->GetAdapter(&dxgiAdapter);
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&m_DxgiFactory));
		dxgiAdapter->Release();
	}

	void DX11RendererContext::CreateSwapChainAndFrameBuffer()
	{
		HRESULT hr = S_OK;

		int fbWidth, fbHeight;
		glfwGetFramebufferSize(m_WindowHandle, &fbWidth, &fbHeight);

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		swapChainDesc.Width = fbWidth; // Defer to WindowWidth
		swapChainDesc.Height = fbHeight; // Defer to WindowHeight
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //FLIP* modes don't support sRGB backbuffer
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = 0;

		hr = m_DxgiFactory->CreateSwapChainForHwnd(m_Device, glfwGetWin32Window(m_WindowHandle), &swapChainDesc, nullptr, nullptr, &m_SwapChain);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create D3D11 Swap Chain For HWND! HRESULT: " << hr << std::endl;
			return;
		}
	}
}