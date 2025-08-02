#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderSceneFromMainCamera(EntityComponentSystem* ECS, LightManager* lightManager, Shader* shader, Shader* depthMapShader, Light* _light)
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

	std::unordered_map<unsigned int, Transform*> transforms = ECS->GetAllComponentsOfType<Transform>();
	std::unordered_map<unsigned int, MeshComponent*> meshComponents = ECS->GetAllComponentsOfType<MeshComponent>();

	light = _light;
	light->FrameSetup(lightManager, ECS->GetObjectComponent<Transform>(mainCamera), depthMapShader, shader);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
			light->RenderObjectToDepthmap(meshComponent->mesh, transformIt->second, depthMapShader);
	}
	light->FrameReset();

	SetShaderFrame(ECS, mainCamera, depthMapShader, shader, light);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
		{
			RenderObject(transformIt->second, meshComponent->mesh, meshComponent->texture, meshComponent->material, meshComponent->shader);
		}
	}
}

void Renderer::RenderObject(Transform* transform, Mesh* mesh, Texture* texture, Material* material, Shader* shader)
{
	shader->BindShader();
	texture->BindTexture(shader);
	mesh->BindMesh();
	light->BindTexture(shader);

	glm::mat4 model = transform->transformMatrix;

	shader->SetMat4("model", model);

	shader->SetVec3("oMaterial.color", material->color);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SetShaderFrame(EntityComponentSystem* ECS, unsigned int camera, Shader* depthmapShader, Shader* shader, Light* _light)
{

	CameraComponent* cameraComponent = ECS->GetObjectComponent<CameraComponent>(camera);
	Transform* transformComponent = ECS->GetObjectComponent<Transform>(camera);

	light = _light;
	shader->BindShader();

	glm::mat4 projection = cameraComponent->m_Camera->GetProjection();
	glm::mat4 view = cameraComponent->m_Camera->GetView(transformComponent);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	light->BindTexture(shader);

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	shader->SetVec3("oLight.position", light->position);
	shader->SetVec3("oLight.direction", light->direction);
	shader->SetVec3("oLight.color", light->color);

	shader->SetFloat("farPlane", cameraComponent->m_Camera->m_FarPlane);

	shader->SetVec3("oViewPosition", transformComponent->position);

	std::vector<float> shadowCascadeLevels = { cameraComponent->m_Camera->m_FarPlane / 50.0f, cameraComponent->m_Camera->m_FarPlane / 25.0f, cameraComponent->m_Camera->m_FarPlane / 10.0f, cameraComponent->m_Camera->m_FarPlane / 2.0f };
	shader->SetInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
}