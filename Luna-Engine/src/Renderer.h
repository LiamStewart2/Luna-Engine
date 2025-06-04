#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderMesh(Camera* camera, Shader* shader, Mesh* mesh);
};