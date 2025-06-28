#pragma once

#include "GLM/glm.hpp"

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

	void FrameSetup(Shader* depthmapShader, Shader* shader);

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;

private:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthmapFBO = 0;
	unsigned int depthmapTextureID = 0;
};