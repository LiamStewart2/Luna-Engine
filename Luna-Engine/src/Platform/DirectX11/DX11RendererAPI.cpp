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

        m_Material = Material(
            glm::vec4(1, 0.3f, 0.3f, 1), 0.2f,
            glm::vec4(1, 1, 1, 1), 0.8);

        m_Light = Light(
            Luna::LightType::Directional,
            glm::vec4(1, 1, 1, 1));
        m_LightTransform = Transform(0, glm::vec3(0));
        m_LightTransform.SetEulerRotation(glm::vec3(glm::radians(0.0f), glm::radians(-90.0f), 0));

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
        D3D11_BUFFER_DESC constantBufferDesc;
        constantBufferDesc.ByteWidth = (glm::ceil(sizeof(ConstantBuffer) / 16)) * 16;
        constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

        hr = m_RenderContext->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
        //hr = m_RenderContext->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, NULL);
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

	}

	void DX11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		
	}
	void DX11RendererAPI::Clear()
	{
		
	}
	void DX11RendererAPI::StartFrame(SceneManager* sceneManager, IFramebuffer* framebuffer, ObjectTransformPairing<Camera>* camera)
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

		_cbData.View = camera->object->GetView(camera->objectTransform);
		_cbData.Projection = camera->object->GetProjection();

		_cbData.LightDirection = glm::normalize(m_LightTransform.Forward());
		_cbData.LightColour = m_Light.m_LightColour;
		_cbData.AmbientColour = m_Material.m_AmbientColour;
		_cbData.AmbientIntensity = m_Material.m_AmbientIntensity;
		_cbData.SpecularColour = m_Material.m_SpecularColour;
        _cbData.CameraPosition = camera->objectTransform->position;
        _cbData.SpecularIntensity = m_Material.m_SpecularIntensity;
	}
	void DX11RendererAPI::EndFrame(SceneManager* sceneManager, IFramebuffer* framebuffer)
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