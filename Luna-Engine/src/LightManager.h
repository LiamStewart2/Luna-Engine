#pragma once

#include <vector>

#include "GLM/glm.hpp"

#include "Camera.h"

class LightManager
{
public:
	LightManager(Camera* _camera) : camera(_camera) {}
	~LightManager() {}

	glm::mat4 GenerateLightSpaceMatrix(const glm::vec3& lightPosition, const glm::vec3& lightDirection);

private:
	Camera* camera;

	std::vector<glm::vec4> GetFrustumCornersWorldSpace();
	glm::vec3 GetCenterOfPoints(const std::vector<glm::vec4>& corners);
};

