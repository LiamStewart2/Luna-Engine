#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderMesh(Camera* camera, Shader* shader, Mesh* mesh)
{
	mesh->BindMesh();
	shader->BindShader();
	
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	shader->SetMat4("model", model);
	shader->SetMat4("view", view);
	shader->SetMat4("projection", projection);

	shader->SetFloat("time", glfwGetTime());

	Light light = { glm::vec3(cos(glfwGetTime()) * 5, 0, sin(glfwGetTime()) * 5) , glm::vec3(1.0)};
	Material material = {glm::vec3(1.0)};

	shader->SetVec3("oLight.position", light.position);
	shader->SetVec3("oLight.color", light.color);
	shader->SetVec3("oMaterial.color", material.color);
	shader->SetVec3("oViewPosition", camera->GetPosition());
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}
