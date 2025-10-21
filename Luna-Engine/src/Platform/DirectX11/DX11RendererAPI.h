#pragma once
#include "../../Renderer/RendererAPI.h"
#include "DX11RendererContext.h"

#include "../../Core/SceneManager.h"
#include "../../Core/Camera.h"
#include "../../Renderer/IFramebuffer.h"
#include "../../Renderer/Light.h"
#include "../../Renderer/Material.h"

struct ID3D11Buffer;
struct ID3D11RasterizerState;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D11PixelShader;

struct ConstantBuffer
{

	glm::vec3 LightDirection;
	float AmbientIntensity;

	glm::vec4 LightColour;
	glm::vec4 AmbientColour;
	glm::vec4 SpecularColour;

	glm::vec3 CameraPosition;
	float SpecularIntensity;

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

		void StartFrame(SceneManager* sceneManager, IFramebuffer* framebuffer, ObjectTransformPairing<Camera>* = nullptr) override;
		void EndFrame(SceneManager* sceneManager, IFramebuffer* framebuffer) override;

		void RenderIndexed(unsigned int count, Transform* transform) override;
	private:
		DX11RendererContext* m_RenderContext = nullptr;

		ID3D11RasterizerState* _fillState;
		ID3D11RasterizerState* _wireframeState;
		ID3D11Buffer* _constantBuffer;

		glm::mat4 _world1;
		glm::mat4 _world2;

		ConstantBuffer _cbData;

		Material m_Material;

		void InitShadersAndInputLayout();
		void InitVertexBuffers();
		void InitPipelineVariables();
		void InitRunTimeData();
	};
}