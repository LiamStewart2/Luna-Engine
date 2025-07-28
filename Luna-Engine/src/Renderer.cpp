#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderSceneFromMainCamera(EntityComponentSystem* ECS, Shader* shader, Shader* depthMapShader, Light* _light)
{
	std::unordered_map<unsigned int, CameraComponent*> cameras = ECS->GetAllComponentsOfType<CameraComponent>();
	CameraComponent* mainCamera = nullptr;
	for (auto& [id, cameraComponent] : cameras)
	{
		auto cameraIt = cameras.find(cameraComponent->gameObject);
		if(cameraIt == cameras.end())
			return;
		else
			mainCamera = cameraComponent;
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

void Renderer::SetShaderFrame(unsigned int camera, Shader* depthmapShader, Shader* shader, Light* _light)
{

	light = _light;
	shader->BindShader();

	glm::mat4 projection = glm::perspective(glm::radians(camera->), (float))
	glm::mat4 view = camera->GetViewMatrix();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	light->BindTexture(shader);

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	shader->SetVec3("oLight.position", light->position);
	shader->SetVec3("oLight.direction", light->direction);
	shader->SetVec3("oLight.color", light->color);

	shader->SetFloat("farPlane", camera->farPlane);

	shader->SetVec3("oViewPosition", camera->GetPosition());

	std::vector<float> shadowCascadeLevels = { camera->farPlane / 50.0f, camera->farPlane / 25.0f, camera->farPlane / 10.0f, camera->farPlane / 2.0f };
	shader->SetInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
}

void Renderer::SetShaderFrame(Camera* camera, Shader* depthmapShader, Shader* shader, Light* _light)
{

	light = _light;
	shader->BindShader();

	glm::mat4 projection = camera->GetProjection();
	glm::mat4 view = camera->GetViewMatrix();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	light->BindTexture(shader);

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	shader->SetVec3("oLight.position", light->position);
	shader->SetVec3("oLight.direction", light->direction);
	shader->SetVec3("oLight.color", light->color);

	shader->SetFloat("farPlane", camera->farPlane);

	shader->SetVec3("oViewPosition", camera->GetPosition());

	std::vector<float> shadowCascadeLevels = { camera->farPlane / 50.0f, camera->farPlane / 25.0f, camera->farPlane / 10.0f, camera->farPlane / 2.0f };
	shader->SetInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
}