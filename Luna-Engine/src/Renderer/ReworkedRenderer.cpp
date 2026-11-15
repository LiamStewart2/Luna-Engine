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
				if(meshComponent.mesh->m_HandleState == AssetHandleState::Loaded)
				{
					IMesh* mesh = meshComponent.mesh->m_MeshReference.get();
					meshComponent.shader->Bind();
					mesh->BindMesh();

					if (meshComponent.material->m_Albedo->m_HandleState == AssetHandleState::Loaded)
						meshComponent.material->m_Albedo->m_TextureReference->BindTexture(0);
					if (meshComponent.material->m_SpecularMap->m_HandleState == AssetHandleState::Loaded)
						meshComponent.material->m_SpecularMap->m_TextureReference->BindTexture(1);
					if (meshComponent.material->m_NormalMap->m_HandleState == AssetHandleState::Loaded)
						meshComponent.material->m_NormalMap->m_TextureReference->BindTexture(3);
					if (meshComponent.material->m_MetallicMap->m_HandleState == AssetHandleState::Loaded)
						meshComponent.material->m_MetallicMap->m_TextureReference->BindTexture(4);
					if (meshComponent.material->m_AOMap->m_HandleState == AssetHandleState::Loaded)
						meshComponent.material->m_AOMap->m_TextureReference->BindTexture(5);

					ShadowRenderer::GetLightFramebuffer()->BindDepthBufferAsTexture(2);

					s_RendererAPI->RenderIndexed(mesh->GetIndexCount(), &transformIt->second, meshComponent.material);

					meshComponent.shader->Unbind();
				}
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
			std::shared_ptr<MeshHandle> mesh = sceneManager->GetAssetManager()->GetMesh(false, "Assets/Models/skybox.obj");
			if(mesh->m_HandleState == AssetHandleState::Loaded)
			{
				s_RendererAPI->StartSkybox(sceneManager);
				sceneManager->GetAssetManager()->GetShader("Assets/Shaders/SkyboxShader/skybox.hlsl")->Bind();
				mesh->m_MeshReference->BindMesh();
				Transform skyboxTransform = Transform(0);
				s_RendererAPI->RenderIndexed(mesh->m_MeshReference->GetIndexCount(), &skyboxTransform, nullptr);
			}
		}
	}
}