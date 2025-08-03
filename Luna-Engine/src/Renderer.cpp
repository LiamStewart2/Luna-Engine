#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderSceneFromMainCamera(EntityComponentSystem* ECS, LightManager* lightManager, Shader* shader, Shader* depthMapShader)
{
	std::unordered_map<unsigned int, CameraComponent*> cameras = ECS->GetAllComponentsOfType<CameraComponent>();
	unsigned int mainCamera = 0;
	for (auto& [id, cameraComponent] : cameras)
	{
		auto cameraIt = cameras.find(cameraComponent->gameObject);
		if(cameraIt == cameras.end())
			return;
		else
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
	
	lightComponent->m_Light.FrameSetup(lightManager, ECS->GetObjectComponent<Transform>(mainCamera), lightTransform, shader);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
			lightComponent->m_Light.RenderObjectToDepthmap(meshComponent->mesh, transformIt->second, depthMapShader);
	}
	lightComponent->m_Light.FrameReset();

	SetShaderFrame(ECS, mainCamera, depthMapShader, shader);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
		{
			RenderObject(transformIt->second, lightComponent, meshComponent->mesh, meshComponent->texture, meshComponent->material, meshComponent->shader);
		}
	}
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

void Renderer::SetShaderFrame(EntityComponentSystem* ECS, unsigned int camera, Shader* depthmapShader, Shader* shader)
{

	CameraComponent* cameraComponent = ECS->GetObjectComponent<CameraComponent>(camera);
	Transform* transformComponent = ECS->GetObjectComponent<Transform>(camera);

	std::unordered_map<unsigned int, LightComponent*> lightComponents = ECS->GetAllComponentsOfType<LightComponent>();
	Transform* lightTransform = nullptr; LightComponent* lightComponent = nullptr;
	for (auto& [id, LC] : lightComponents)
	{
		lightTransform = ECS->GetObjectComponent<Transform>(id);
		lightComponent = LC;
	}

	shader->BindShader();

	glm::mat4 projection = cameraComponent->m_Camera->GetProjection();
	glm::mat4 view = cameraComponent->m_Camera->GetView(transformComponent);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	if(lightTransform != nullptr && lightComponent != nullptr)
	{
		lightComponent->m_Light.BindTexture(shader);
		shader->SetVec3("oLight.position", lightTransform->position);
		shader->SetVec3("oLight.direction", lightTransform->Forward());
		shader->SetVec3("oLight.color", lightComponent->m_LightColor);
	}

	shader->SetFloat("farPlane", cameraComponent->m_Camera->m_FarPlane);

	shader->SetVec3("oViewPosition", transformComponent->position);

	std::vector<float> shadowCascadeLevels = { cameraComponent->m_Camera->m_FarPlane / 50.0f, cameraComponent->m_Camera->m_FarPlane / 25.0f, cameraComponent->m_Camera->m_FarPlane / 10.0f, cameraComponent->m_Camera->m_FarPlane / 2.0f };
	shader->SetInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
}