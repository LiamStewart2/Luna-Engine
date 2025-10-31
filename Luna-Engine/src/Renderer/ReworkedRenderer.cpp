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
		if (camera->object == nullptr)
		{
			std::unordered_map<unsigned int, CameraComponent*> cameras = sceneManager->GetCurrentScene()->GetECS()->GetAllComponentsOfType<CameraComponent>();
			unsigned int mainCameraID = 0;

			for (auto& [id, cameraComponent] : cameras)
			{
				auto cameraIt = cameras.find(cameraComponent->gameObject);
				if (cameraIt == cameras.end())
					return;
				else if (cameraComponent->m_MainCamera)
				{
					mainCameraID = id;
					camera->object = cameraComponent->m_Camera;
					camera->objectTransform = sceneManager->GetCurrentScene()->GetECS()->GetObjectComponent<Transform>(id);
				}
			}
			if (mainCameraID == 0)
			{
				std::cerr << "NO MAIN CAMERA" << std::endl;
				return;
			}
		}

		ShadowRenderer::ClearFramebuffer();
		ShadowRenderer::ShadowPass(s_RendererAPI, sceneManager, camera);


		s_RendererAPI->StartFrame(sceneManager,framebuffer, camera);
	}
	void ReworkedRenderer::EndFrame(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		s_RendererAPI->EndFrame(sceneManager, framebuffer);
	}
	void ReworkedRenderer::Render(SceneManager* sceneManager, IFramebuffer* framebuffer, ObjectTransformPairing<Camera>* camera)
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
				ShadowRenderer::GetLightFramebuffer()->BindDepthBufferAsTexture(2);

				s_RendererAPI->RenderIndexed(meshComponent->mesh->GetIndexCount(), transformIt->second);
			}
		}

		RenderSkybox(sceneManager, framebuffer);

		framebuffer->Unbind();
	}

	void ReworkedRenderer::RenderSkybox(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		std::unordered_map<unsigned int, CameraComponent*> cameras = sceneManager->GetCurrentScene()->GetECS()->GetAllComponentsOfType<CameraComponent>();
		unsigned int mainCameraID = 0;

		for (auto& [id, cameraComponent] : cameras)
		{
			auto cameraIt = cameras.find(cameraComponent->gameObject);
			if (cameraIt == cameras.end())
				return;
			else if (cameraComponent->m_MainCamera)
				mainCameraID = id;
		}
		if (mainCameraID == 0)
		{
			std::cerr << "NO MAIN CAMERA" << std::endl;
			return;
		}

		if (cameras[mainCameraID]->m_UseSkybox)
		{
			s_RendererAPI->StartSkybox(sceneManager);
			std::shared_ptr<IMesh> mesh = sceneManager->GetAssetManager()->GetMesh("Assets/Models/planeobj.obj");
			sceneManager->GetAssetManager()->GetShader("Assets/Shaders/SkyboxShader/skybox.hlsl")->Bind();
			mesh->BindMesh();
			cameras[mainCameraID]->m_SkyboxTexture->BindTexture(0);
			Transform skyboxTransform = Transform(0);
			s_RendererAPI->RenderIndexed(mesh->GetIndexCount(), &skyboxTransform);
		}
	}
}