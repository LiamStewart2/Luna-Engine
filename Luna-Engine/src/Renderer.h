#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"
#include "ECS/CameraComponent.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderSceneFromMainCamera(EntityComponentSystem* ECS, Shader* depthMapShader, Light* _light);

	void SetShaderFrame(Camera* camera, Shader* depthmapShader, Shader* shader, Light* _light);
	void RenderObject(Transform* transform, Mesh* mesh, Texture* texture, Material* material, Shader* shader);

	Light* light;
};