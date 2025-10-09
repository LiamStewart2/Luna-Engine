#include "DX11RendererAPI.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <d3d11_4.h>
#include <d3dcompiler.h>

namespace Luna
{
    DX11RendererAPI::~DX11RendererAPI()
    {
        if (_fillState)_fillState->Release();
        if (_wireframeState)_wireframeState->Release();
        if (_constantBuffer)_constantBuffer->Release();
    }

	void DX11RendererAPI::Init(std::shared_ptr<RendererContext> renderContext)
	{
		// Initialization code for DirectX 11 Renderer API
		m_RenderContext = (DX11RendererContext*)(renderContext.get());

        InitShadersAndInputLayout();
        InitVertexBuffers();
        InitPipelineVariables();
        InitRunTimeData();
	}

	void DX11RendererAPI::InitShadersAndInputLayout()
	{
        
	}

    void DX11RendererAPI::InitVertexBuffers()
    {
        HRESULT hr = S_OK;

    }

    void DX11RendererAPI::InitPipelineVariables()
    {
        HRESULT hr = S_OK;

        //Rasterizer
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;

        hr = m_RenderContext->GetDevice()->CreateRasterizerState(&rasterizerDesc, &_fillState);
        if (FAILED(hr))
        {
			std::cout << "Line 160 Failed To Create Rasterizer State" << std::endl;
            return;
        }

        m_RenderContext->GetImmediateContext()->RSSetState(_fillState);

        // Wireframe State
        D3D11_RASTERIZER_DESC wireframeDesc = {};
        wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
        wireframeDesc.CullMode = D3D11_CULL_FRONT;

        hr = m_RenderContext->GetDevice()->CreateRasterizerState(&wireframeDesc, &_wireframeState);
        if (FAILED(hr)) 
        {
            std::cout << "Line 174 Failed To Create Wireframe State" << std::endl;
            return;
        }

        //m_RenderContext->GetImmediateContext()->RSSetState(_wireframeState);

        //Constant Buffer
        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
        constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        hr = m_RenderContext->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
        if (FAILED(hr))
        {
            std::cout << "Line 190 Failed To Create Constant Buffer" << std::endl;
            return;
        }

        m_RenderContext->GetImmediateContext()->VSSetConstantBuffers(0, 1, &_constantBuffer);
        m_RenderContext->GetImmediateContext()->PSSetConstantBuffers(0, 1, &_constantBuffer);
    }

    void DX11RendererAPI::InitRunTimeData()
    {
        float aspect = 16 / 8.7;

        glm::vec3 Eye = glm::vec3(0, 0, -3.0f);
        glm::vec3 At = glm::vec3(0, 0, 0);
        glm::vec3 Up = glm::vec3(0, 1, 0);

		_cbData.View = (glm::lookAt(Eye, At, Up));
		_cbData.Projection = (glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f));
	}

	void DX11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		
	}
	void DX11RendererAPI::Clear()
	{
		
	}
	void DX11RendererAPI::StartFrame()
	{
        /*
        _world1 = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));
        _world2 = glm::translate(glm::mat4(1), glm::vec3(2, 0, 2));
        _world2 = glm::rotate(_world2, (float)glfwGetTime(), glm::vec3(0, 0, 1));
        _cbData.World = _world1;

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        m_RenderContext->GetImmediateContext()->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        m_RenderContext->GetImmediateContext()->Unmap(_constantBuffer, 0);

        RenderIndexed(12 * 3);

        // Second Cube
		_cbData.World = _world2;

        m_RenderContext->GetImmediateContext()->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        m_RenderContext->GetImmediateContext()->Unmap(_constantBuffer, 0);

		RenderIndexed(12 * 3);
        */

	}
	void DX11RendererAPI::EndFrame()
	{
		// Code to end the current frame
	}
	void DX11RendererAPI::RenderIndexed(unsigned int count, Transform* transform)
	{
		// Render indexed geometry

		_cbData.World = transform->transformMatrix;

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        m_RenderContext->GetImmediateContext()->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        m_RenderContext->GetImmediateContext()->Unmap(_constantBuffer, 0);

        m_RenderContext->GetImmediateContext()->DrawIndexed(count, 0, 0);
	}
}