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
        if (_vertexShader)_vertexShader->Release();
        if (_inputLayout)_inputLayout->Release();
        if (_pixelShader)_pixelShader->Release();
        if (_constantBuffer)_constantBuffer->Release();
        if (_vertexBuffer)_vertexBuffer->Release();
        if (_indexBuffer)_indexBuffer->Release();
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
        HRESULT hr = S_OK;
        ID3DBlob* errorBlob;

        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        // Set the D3DCOMPILE_DEBUG flag and D3DCOMPILE_SKIP_OPTIMIZATION
        // This means the shader runs slower, but debugging is easier
        // As optimizations will move/remove lines of code in the shader
        // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/d3dcompile-constants
        dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ID3DBlob* vsBlob;

        hr = D3DCompileFromFile(L"Assets/Shaders/SimpleShader/SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
        if (FAILED(hr))
        {
			std::cerr << "Shader Failed To Compile" << std::endl;
            return;
        }

        hr = m_RenderContext->GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);

        if (FAILED(hr))
        {
            std::cerr << "Vertex Shader Failed To Compile" << std::endl;
            return;
        }

        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
        };

        hr = m_RenderContext->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
        if (FAILED(hr)) 
        {
            std::cerr << "Input Layout Failed To Compile" << std::endl;
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////

        ID3DBlob* psBlob;

        hr = D3DCompileFromFile(L"Assets/Shaders/SimpleShader/SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
        if (FAILED(hr))
        {
            std::cerr << "line 68 Failed To Compile" << std::endl;
            return;
        }

        hr = m_RenderContext->GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);

        vsBlob->Release();
        psBlob->Release();
	}

    void DX11RendererAPI::InitVertexBuffers()
    {
        HRESULT hr = S_OK;

        SimpleVertex VertexData[] =
        {
            //Position                          //Color             
            { glm::vec3(-1.00f,  1.00f,  1),    glm::vec4(1.0f,  0.0f, 0.0f,  1.0f)},
            { glm::vec3(1.00f,   1.00f,  1),    glm::vec4(0.0f,  1.0f, 0.0f,  1.0f)},
            { glm::vec3(-1.00f, -1.00f,  1),    glm::vec4(0.0f,  0.0f, 1.0f,  1.0f)},
            { glm::vec3(1.00f,  -1.00f,  1),    glm::vec4(1.0f,  1.0f, 1.0f,  1.0f)},
            { glm::vec3(-1.00f,  1.00f, -1),    glm::vec4(1.0f,  0.0f, 0.0f,  1.0f)},
            { glm::vec3(1.00f,   1.00f, -1),    glm::vec4(0.0f,  1.0f, 0.0f,  1.0f)},
            { glm::vec3(-1.00f, -1.00f, -1),    glm::vec4(0.0f,  0.0f, 1.0f,  1.0f)},
            { glm::vec3(1.00f,  -1.00f, -1),    glm::vec4(1.0f,  1.0f, 1.0f,  1.0f)},
        };

        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(VertexData);
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

        hr = m_RenderContext->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
        if (FAILED(hr))
        {
			std::cerr << "Failed to create vertex buffer" << std::endl;
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////

        WORD IndexData[] =
        {
            //Indices
            0, 1, 2,
            2, 1, 3,
            4, 5, 0,
            5, 1, 0,
            5, 4, 6,
            5, 6, 7,
            2, 7, 6,
            7, 2, 3,
            4, 0, 6,
            0, 2, 6,
            1, 5, 7,
            7, 3, 1
        };

        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = sizeof(IndexData);
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA indexData = { IndexData };

        hr = m_RenderContext->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
        if (FAILED(hr)) 
        {
            std::cerr << "Failed to create index buffer" << std::endl;
            return;
        }
    }

    void DX11RendererAPI::InitPipelineVariables()
    {
        HRESULT hr = S_OK;

        //Input Assembler
        m_RenderContext->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_RenderContext->GetImmediateContext()->IASetInputLayout(_inputLayout);

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
        float aspect = m_RenderContext->GetViewport()->Width / m_RenderContext->GetViewport()->Height;

        glm::vec3 Eye = glm::vec3(0, 0, -3.0f);
        glm::vec3 At = glm::vec3(0, 0, 0);
        glm::vec3 Up = glm::vec3(0, 1, 0);

		_cbData.View = (glm::lookAt(Eye, At, Up));
		_cbData.Projection = (glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f));
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
        _cbData.World = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        m_RenderContext->GetImmediateContext()->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        m_RenderContext->GetImmediateContext()->Unmap(_constantBuffer, 0);

        UINT stride = { sizeof(SimpleVertex) };
        UINT offset = 0;
        m_RenderContext->GetImmediateContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
        m_RenderContext->GetImmediateContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

        m_RenderContext->GetImmediateContext()->VSSetShader(_vertexShader, nullptr, 0);
        m_RenderContext->GetImmediateContext()->PSSetShader(_pixelShader, nullptr, 0);

        ID3D11RenderTargetView* rtv = m_RenderContext->GetRenderTargetView();
        m_RenderContext->GetImmediateContext()->OMSetRenderTargets(1, &rtv, nullptr);
	}
	void DX11RendererAPI::EndFrame()
	{
		// Code to end the current frame
	}
	void DX11RendererAPI::RenderIndexed(unsigned int count)
	{
		// Render indexed geometry

        m_RenderContext->GetImmediateContext()->DrawIndexed(12 * 3, 0, 0);
	}
}