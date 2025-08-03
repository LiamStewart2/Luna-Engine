#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"
#include "ECS/CameraComponent.h"
#include "ECS/LightComponent.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderSceneFromMainCamera(EntityComponentSystem* ECS, LightManager* lightManager, Shader* shader, Shader* depthMapShader, Light* _light);

	void SetShaderFrame(EntityComponentSystem* ECS, unsigned int camera, Shader* depthmapShader, Shader* shader, Light* _light);
	void RenderObject(Transform* transform, Mesh* mesh, Texture* texture, Material* material, Shader* shader);

	Light* light;
};