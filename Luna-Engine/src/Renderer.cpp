#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderObject(Transform* transform, Mesh* mesh, Texture* texture, Material* material, Shader* shader)
{
	shader->BindShader();
	texture->BindTexture(shader);
	mesh->BindMesh();
	light->BindTexture(shader);

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, transform->position);
	
	model = glm::rotate(model, glm::radians(transform->rotation.x), {1, 0, 0});
	model = glm::rotate(model, glm::radians(transform->rotation.y), {0, 1, 0});
	model = glm::rotate(model, glm::radians(transform->rotation.z), {0, 0, 1});

	model = glm::scale(model, transform->scale);

	shader->SetMat4("model", model);

	shader->SetVec3("oMaterial.color", material->color);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SetShaderFrame(Camera* camera, Shader* depthmapShader, Shader* shader, Light* _light)
{
	light = _light;
	shader->BindShader();

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	shader->SetVec3("oLight.position", light->position);
	shader->SetVec3("oLight.direction", light->direction);
	shader->SetVec3("oLight.color", light->color);

	shader->SetVec3("oViewPosition", camera->GetPosition());
}