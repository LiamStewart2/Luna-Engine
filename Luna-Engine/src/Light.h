#pragma once

#include "GLM/glm.hpp"

#include "Globals.h"
#include "Shader.h"
#include "Mesh.h"
#include "ECS/Transform.h"

class Light
{
public:
	Light(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _color);
	~Light();


	void BuildLight();
	void RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader);

	void BindTexture(Shader* shader);
	void FrameSetup(Shader* depthmapShader, Shader* shader);
	void FrameReset();

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;

private:
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	unsigned int depthmapFBO = 0;
	unsigned int depthmapTextureID = 0;

	glm::mat4 lightSpaceMatrix = glm::mat4(1);
};