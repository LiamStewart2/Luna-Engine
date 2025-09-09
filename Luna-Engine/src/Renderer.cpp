#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderSceneFromMainCamera(EntityComponentSystem* ECS, LightManager* lightManager, Shader* shader, Shader* depthMapShader, FrameBuffer* framebuffer)
{
	std::unordered_map<unsigned int, CameraComponent*> cameras = ECS->GetAllComponentsOfType<CameraComponent>();
	unsigned int mainCamera = 0;
	for (auto& [id, cameraComponent] : cameras)
	{
		auto cameraIt = cameras.find(cameraComponent->gameObject);
		if(cameraIt == cameras.end())
			return;
		else if(cameraComponent->m_MainCamera)
			mainCamera = id;
	}

	if (mainCamera == 0)
	{
		std::cerr << "NO MAIN CAMERA" << std::endl;
		return;
	}

	std::unordered_map<unsigned int, LightComponent*> lightComponents = ECS->GetAllComponentsOfType<LightComponent>();
	Transform* lightTransform = nullptr; LightComponent* lightComponent = nullptr;
	for (auto& [id, LC] : lightComponents)
	{
		lightComponent = LC;
		lightTransform = ECS->GetObjectComponent<Transform>(id);
	}

	std::unordered_map<unsigned int, Transform*> transforms = ECS->GetAllComponentsOfType<Transform>();
	std::unordered_map<unsigned int, MeshComponent*> meshComponents = ECS->GetAllComponentsOfType<MeshComponent>();
	
	lightManager->InitCascadeLevels(ECS->GetObjectComponent<CameraComponent>(mainCamera)->m_Camera);
	lightComponent->m_Light.FrameSetup(lightManager, ECS->GetObjectComponent<CameraComponent>(mainCamera)->m_Camera, ECS->GetObjectComponent<Transform>(mainCamera), lightTransform, shader, framebuffer);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
			lightComponent->m_Light.RenderObjectToDepthmap(meshComponent->mesh, transformIt->second, depthMapShader);
	}
	lightComponent->m_Light.FrameReset();

	SetShaderFrame(ECS, ECS->GetObjectComponent<CameraComponent>(mainCamera)->m_Camera, ECS->GetObjectComponent<Transform>(mainCamera), depthMapShader, shader, framebuffer);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
		{
			RenderObject(transformIt->second, lightComponent, meshComponent->mesh, meshComponent->texture, meshComponent->material, meshComponent->shader);
		}
	}

	if (framebuffer != nullptr)
		framebuffer->Unbind();
}

void Renderer::EditorRenderPass(EntityComponentSystem* ECS, LightManager* lightManager, EditorCamera* camera, Shader* shader, Shader* depthMapShader, FrameBuffer* framebuffer)
{
	if(ECS == nullptr) {std::cout << "NO ECS PASSED" << std::endl; return; }
	if(camera == nullptr) {std::cout << "NO CAMERA PASSED" << std::endl; return; }
	if(shader == nullptr) {std::cout << "NO SHADER PASSED" << std::endl; return; }
	if(framebuffer == nullptr) {std::cout << "NO FRAMEBUFFER PASSED" << std::endl; return; }

	std::unordered_map<unsigned int, LightComponent*> lightComponents = ECS->GetAllComponentsOfType<LightComponent>();
	Transform* lightTransform = nullptr; LightComponent* lightComponent = nullptr;
	for (auto& [id, LC] : lightComponents)
	{
		lightComponent = LC;
		lightTransform = ECS->GetObjectComponent<Transform>(id);
	}

	std::unordered_map<unsigned int, Transform*> transforms = ECS->GetAllComponentsOfType<Transform>();
	std::unordered_map<unsigned int, MeshComponent*> meshComponents = ECS->GetAllComponentsOfType<MeshComponent>();

	lightManager->InitCascadeLevels((Camera*)camera);
	Transform cameraTransform = Transform(0, camera->m_Position, camera->m_Rotation);
	lightComponent->m_Light.FrameSetup(lightManager, (Camera*)camera, &cameraTransform, lightTransform, shader, framebuffer);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
			lightComponent->m_Light.RenderObjectToDepthmap(meshComponent->mesh, transformIt->second, depthMapShader);
	}
	lightComponent->m_Light.FrameReset();

	SetShaderFrame(ECS, (Camera*)camera, &cameraTransform, depthMapShader, shader, framebuffer);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
		{
			RenderObject(transformIt->second, lightComponent, meshComponent->mesh, meshComponent->texture, meshComponent->material, meshComponent->shader);
		}
	}

	if (framebuffer != nullptr)
		framebuffer->Unbind();
}

void Renderer::RenderObject(Transform* transform, LightComponent* light, Mesh* mesh, Texture* texture, Material* material, Shader* shader)
{
	shader->BindShader();
	texture->BindTexture(shader);
	mesh->BindMesh();
	light->m_Light.BindTexture(shader);

	glm::mat4 model = transform->transformMatrix;

	shader->SetMat4("model", model);

	shader->SetVec3("oMaterial.color", material->color);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SetShaderFrame(EntityComponentSystem* ECS, Camera* camera, Transform* cameraTransform, Shader* depthmapShader, Shader* shader, FrameBuffer* framebuffer)
{
	if (framebuffer != nullptr)
		framebuffer->Bind();
	else
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);
	glClearColor(0.5f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::unordered_map<unsigned int, LightComponent*> lightComponents = ECS->GetAllComponentsOfType<LightComponent>();
	Transform* lightTransform = nullptr; LightComponent* lightComponent = nullptr;
	for (auto& [id, LC] : lightComponents)
	{
		lightTransform = ECS->GetObjectComponent<Transform>(id);
		lightComponent = LC;
	}

	shader->BindShader();

	glm::mat4 projection = camera->GetProjection();
	glm::mat4 view = camera->GetView(cameraTransform);

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	if(lightTransform != nullptr && lightComponent != nullptr)
	{
		lightComponent->m_Light.BindTexture(shader);
		shader->SetVec3("oLight.position", lightTransform->position);
		shader->SetVec3("oLight.direction", lightTransform->Forward());
		shader->SetVec3("oLight.color", lightComponent->m_LightColor);
	}

	shader->SetFloat("farPlane", camera->m_FarPlane);

	shader->SetVec3("oViewPosition", cameraTransform->position);

	std::vector<float> shadowCascadeLevels = { camera->m_FarPlane / 50.0f, camera->m_FarPlane / 25.0f, camera->m_FarPlane / 10.0f, camera->m_FarPlane / 2.0f };
	shader->SetInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
}