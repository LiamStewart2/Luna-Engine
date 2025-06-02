#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderMesh(Mesh* mesh);
};