#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

#include "Scene.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderScene(Scene* scene, Camera* camera, Light* light, Shader* shader, Mesh* mesh, Material* material);

private:
	void SetupFrame(Camera* camera, Shader* shader, Light* light);
	void RenderMesh(Camera* camera, Shader* shader, Mesh* mesh, Material* material);
};