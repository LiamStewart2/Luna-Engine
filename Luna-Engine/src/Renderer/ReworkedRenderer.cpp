#include "ReworkedRenderer.h"

namespace Luna
{
	std::shared_ptr<IRendererAPI> ReworkedRenderer::s_RendererAPI = IRendererAPI::Create();;

	void ReworkedRenderer::Init(std::shared_ptr<RendererContext> renderContext)
	{
		s_RendererAPI->Init(renderContext);
		ShadowRenderer::InitFramebuffer();
	}
	void ReworkedRenderer::Shutdown()
	{
	}
	void ReworkedRenderer::BeginFrame(SceneManager* sceneManager, IFramebuffer* framebuffer, ObjectTransformPairing<Camera>* camera)
	{
		s_RendererAPI->StartFrame(sceneManager,framebuffer, camera);
		ShadowRenderer::ClearFramebuffer();
		ShadowRenderer::ShadowPass(s_RendererAPI, sceneManager);
	}
	void ReworkedRenderer::EndFrame(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		s_RendererAPI->EndFrame(sceneManager, framebuffer);
	}
	void ReworkedRenderer::Render(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		// BIND EVERYTHING HERE
		framebuffer->Bind();

		Scene* scene = sceneManager->GetCurrentScene();
		EntityComponentSystem* ECS = scene->GetECS();

		std::unordered_map<unsigned int, MeshComponent*> meshComponents = ECS->GetAllComponentsOfType<MeshComponent>();
		std::unordered_map<unsigned int, Transform*> transforms = ECS->GetAllComponentsOfType<Transform>();
		for (auto& [id, meshComponent] : meshComponents)
		{
			auto transformIt = transforms.find(meshComponent->gameObject);
			if (transformIt != transforms.end())
			{
				meshComponent->shader->Bind();
				meshComponent->mesh->BindMesh();
				meshComponent->texture->BindTexture(0);
				meshComponent->specularMap->BindTexture(1);
				
				s_RendererAPI->RenderIndexed(meshComponent->mesh->GetIndexCount(), transformIt->second);
			}
		}

		framebuffer->Unbind();
	}
}