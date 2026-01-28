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
			std::unordered_map<unsigned int, CameraComponent>& cameras = sceneManager->GetCurrentScene()->GetECS()->GetAllComponentsOfType<CameraComponent>();
			unsigned int mainCameraID = 0;

			for (auto& [id, cameraComponent] : cameras)
			{
				auto cameraIt = cameras.find(cameraComponent.gameObject);
				if (cameraIt == cameras.end())
					return;
				else if (cameraComponent.m_MainCamera)
				{
					mainCameraID = id;
					camera->object = cameraComponent.m_Camera;
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

		std::unordered_map<unsigned int, MeshComponent>& meshComponents = ECS->GetAllComponentsOfType<MeshComponent>();
		std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
		for (auto& [id, meshComponent] : meshComponents)
		{
			auto transformIt = transforms.find(meshComponent.gameObject);
			if (transformIt != transforms.end())
			{
				meshComponent.shader->Bind();
				if(meshComponent.mesh)
					meshComponent.mesh->BindMesh();
				
				if(meshComponent.material->m_Albedo != nullptr)
					meshComponent.material->m_Albedo->BindTexture(0);
				if(meshComponent.material->m_SpecularMap != nullptr)
					meshComponent.material->m_SpecularMap->BindTexture(1);
				if(meshComponent.material->m_NormalMap != nullptr)
					meshComponent.material->m_NormalMap->BindTexture(3);
				if(meshComponent.material->m_MetallicMap != nullptr)
					meshComponent.material->m_MetallicMap->BindTexture(4);
				if(meshComponent.material->m_AOMap != nullptr)
					meshComponent.material->m_AOMap->BindTexture(5);

				ShadowRenderer::GetLightFramebuffer()->BindDepthBufferAsTexture(2);

				s_RendererAPI->RenderIndexed(meshComponent.mesh->GetIndexCount(), &transformIt->second, meshComponent.material);

				meshComponent.shader->Unbind();
			}
		}

		RenderSkybox(sceneManager, framebuffer);

		framebuffer->Unbind();
	}

	void ReworkedRenderer::RenderSkybox(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		std::unordered_map<unsigned int, CameraComponent>& cameras = sceneManager->GetCurrentScene()->GetECS()->GetAllComponentsOfType<CameraComponent>();
		unsigned int mainCameraID = 0;

		for (auto& [id, cameraComponent] : cameras)
		{
			auto cameraIt = cameras.find(cameraComponent.gameObject);
			if (cameraIt == cameras.end())
				return;
			else if (cameraComponent.m_MainCamera)
				mainCameraID = id;
		}
		if (mainCameraID == 0)
		{
			std::cerr << "NO MAIN CAMERA" << std::endl;
			return;
		}

		if (cameras[mainCameraID].m_UseSkybox)
		{
			s_RendererAPI->StartSkybox(sceneManager);
			std::shared_ptr<IMesh> mesh = sceneManager->GetAssetManager()->GetMesh("Assets/Models/skybox.obj");
			sceneManager->GetAssetManager()->GetShader("Assets/Shaders/SkyboxShader/skybox.hlsl")->Bind();
			mesh->BindMesh();
			Transform skyboxTransform = Transform(0);
			s_RendererAPI->RenderIndexed(mesh->GetIndexCount(), &skyboxTransform, nullptr);
		}
	}
}