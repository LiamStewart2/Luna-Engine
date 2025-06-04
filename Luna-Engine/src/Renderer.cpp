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
	glm::translate(model, glm::vec3(0, 0, 2));

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 1000.0f);

	shader->SetMat4("model", model);
	shader->SetMat4("view", view);
	shader->SetMat4("projection", projection);

	shader->SetFloat("time", glfwGetTime());

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cerr << "GL ERROR: " << err << std::endl;
}
