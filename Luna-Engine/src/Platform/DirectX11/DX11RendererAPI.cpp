#include "DX11RendererAPI.h"

#include <d3d11_4.h>

namespace Luna
{
	void DX11RendererAPI::Init(std::shared_ptr<RendererContext> renderContext)
	{
		// Initialization code for DirectX 11 Renderer API
		m_RenderContext = static_cast<DX11RendererContext*>(renderContext.get());
	}
	void DX11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		// Set the clear color for the renderer
		m_RenderContext->GetImmediateContext()->ClearRenderTargetView(m_RenderContext->GetRenderTargetView(), reinterpret_cast<const float*>(&color));
	}
	void DX11RendererAPI::Clear()
	{
		// Clear the screen with the set clear color
		float backgroundColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
		m_RenderContext->GetImmediateContext()->ClearRenderTargetView(m_RenderContext->GetRenderTargetView(), backgroundColor);
	}
	void DX11RendererAPI::StartFrame()
	{
		// Code to start a new frame
	}
	void DX11RendererAPI::EndFrame()
	{
		// Code to end the current frame
	}
	void DX11RendererAPI::RenderIndexed(unsigned int count)
	{
		// Render indexed geometry
	}
}