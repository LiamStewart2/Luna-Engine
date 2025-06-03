#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderMesh(Shader* shader, Mesh* mesh)
{
	shader->BindShader();
	mesh->BindMesh();

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
