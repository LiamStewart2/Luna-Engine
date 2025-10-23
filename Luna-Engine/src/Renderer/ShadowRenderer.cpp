#include "ShadowRenderer.h"

namespace Luna
{
	std::shared_ptr<IFramebuffer> ShadowRenderer::s_LightFramebuffer = nullptr;

	ShadowRenderer::ShadowRenderer()
	{
	}
	ShadowRenderer::~ShadowRenderer()
	{

	}

	void ShadowRenderer::InitFramebuffer()
	{
		FramebufferSpecification framebufferSpec;
		framebufferSpec.m_Width = 2048; framebufferSpec.m_Height = 2048;
		framebufferSpec.m_ColorAttachments = { RGBA16F };
		framebufferSpec.m_DepthAttachment = DEPTH24STENCIL8;

		s_LightFramebuffer = IFramebuffer::Create(framebufferSpec);
	}

	void ShadowRenderer::ClearFramebuffer()
	{
		float backgroundColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		s_LightFramebuffer->Clear(backgroundColor);
	}

	void ShadowRenderer::ShadowPass(std::shared_ptr<IRendererAPI> rendererAPI, SceneManager* sceneManager)
	{
		s_LightFramebuffer->Bind();

		rendererAPI->StartShadowPass(sceneManager);
		Scene* scene = sceneManager->GetCurrentScene();
		EntityComponentSystem* ECS = scene->GetECS();

		sceneManager->GetAssetManager()->GetShader("Assets/Shaders/NewDepthShader/DepthShader.hlsl")->Bind();
		std::unordered_map<unsigned int, MeshComponent*> meshComponents = ECS->GetAllComponentsOfType<MeshComponent>();
		std::unordered_map<unsigned int, Transform*> transforms = ECS->GetAllComponentsOfType<Transform>();
		for (auto& [id, meshComponent] : meshComponents)
		{
			auto transformIt = transforms.find(meshComponent->gameObject);
			if (transformIt != transforms.end())
			{
				meshComponent->mesh->BindMesh();

				rendererAPI->RenderIndexed(meshComponent->mesh->GetIndexCount(), transformIt->second);
			}
		}

		sceneManager->GetAssetManager()->GetShader("Assets/Shaders/NewDepthShader/DepthShader.hlsl")->Unbind();
		s_LightFramebuffer->Unbind();
	}
}