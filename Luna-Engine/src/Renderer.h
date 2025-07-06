#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

#include "ECS/TransformComponent.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void SetShaderFrame(Camera* camera, Shader* depthmapShader, Shader* shader, Light* _light);
	void RenderObject(Transform* transform, Mesh* mesh, Texture* texture, Material* material, Shader* shader);

	Light* light;
};