#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
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

void Renderer::RenderScene(Scene* scene, Camera* camera, Light* light, Shader* shader, Mesh* mesh, Material* material)
{
	SetupFrame(camera, shader, light);

	std::vector<Mesh*>* objectBuffer = scene->getObjectBuffer();
	for (size_t i = 0; i < objectBuffer->size(); i++)
		RenderMesh(camera, shader, mesh, material);
}
