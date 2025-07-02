#pragma once

#include "GLM/glm.hpp"

#include "LightManager.h"
#include "Globals.h"
#include "Shader.h"
#include "Mesh.h"
#include "ECS/Transform.h"

class Light
{
public:
	Light(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _color);
	~Light();


	void BuildLight(LightManager* lightManager);
	void RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader);

	void BindTexture(Shader* shader);
	void FrameSetup(LightManager* lightManager, Shader* depthmapShader, Shader* shader);
	void FrameReset();

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthmapFBO = 0;
	unsigned int matricesUBO = 0;
	unsigned int depthmapsTextureID = 0;

	glm::mat4 lightSpaceMatrix = glm::mat4(1);
private:
};