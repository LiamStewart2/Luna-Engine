#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderMesh(Camera* camera, Shader* shader, Mesh* mesh)
{
	shader->BindShader();
	mesh->BindMesh();
	
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4();
	glm::mat4 persperctive = glm::perspective(60.0f, 16.0f/9.0f, 0.1f, 1000.0f);

	shader->SetMat4("view", view);
	shader->SetMat4("model", model);
	shader->SetMat4("persperctive", persperctive);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}
