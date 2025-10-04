#pragma once
#include "../../Renderer-2/RendererAPI.h"
#include "DX11RendererContext.h"

struct ID3D11Buffer;
struct ID3D11RasterizerState;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D11PixelShader;

struct SimpleVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
};

struct ConstantBuffer
{
	glm::mat4x4 Projection;
	glm::mat4x4 View;
	glm::mat4x4 World;
};

namespace Luna
{
	class DX11RendererAPI : public IRendererAPI
	{
	public:
		virtual ~DX11RendererAPI();

		void Init(std::shared_ptr<RendererContext> renderContext) override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void StartFrame() override;
		void EndFrame() override;

		void RenderIndexed(unsigned int count) override;
	private:
		DX11RendererContext* m_RenderContext = nullptr;

		ID3D11RasterizerState* _fillState;
		ID3D11RasterizerState* _wireframeState;
		ID3D11VertexShader* _vertexShader;
		ID3D11InputLayout* _inputLayout;
		ID3D11PixelShader* _pixelShader;
		ID3D11Buffer* _constantBuffer;
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;

		glm::mat4 _world1;
		glm::mat4 _world2;

		ConstantBuffer _cbData;

		void InitShadersAndInputLayout();
		void InitVertexBuffers();
		void InitPipelineVariables();
		void InitRunTimeData();
	};
}