#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Shader.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderMesh(Shader* shader, Mesh* mesh);
};