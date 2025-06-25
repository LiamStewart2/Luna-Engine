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
	mesh->BindMesh();
	texture->BindTexture(shader);

	glm::mat4 model = glm::mat4(1.0f);

	glm::scale(model, transform->scale);
	
	glm::rotate(model, glm::radians(transform->rotation.x), {1, 0, 0});
	glm::rotate(model, glm::radians(transform->rotation.y), {0, 1, 0});
	glm::rotate(model, glm::radians(transform->rotation.z), {0, 0, 1});

	glm::translate(model, transform->position);

	shader->SetMat4("model", model);

	shader->SetVec3("oMaterial.color", material->color);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SetupFrame(Camera* camera, Shader* shader, Light* light)
{
	shader->BindShader();

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	shader->SetVec3("oLight.position", light->position);
	shader->SetVec3("oLight.color", light->color);

	shader->SetVec3("oViewPosition", camera->GetPosition());
}

void Renderer::RenderMesh(Camera* camera, Shader* shader, Mesh* mesh, Material* material)
{
	mesh->BindMesh();
	shader->BindShader();
	
	glm::mat4 model = glm::mat4(1.0f);

	shader->SetMat4("model", model);

	shader->SetVec3("oMaterial.color", material->color);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}
