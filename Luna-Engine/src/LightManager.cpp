#include "LightManager.h"

std::vector<glm::mat4> LightManager::GenerateLightSpaceMatrices(Transform* cameraTransform, const glm::vec3& lightDirection)
{
	std::vector<glm::mat4> matrices;

	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; i++)
	{
		if (i == 0)
			matrices.push_back(GenerateLightSpaceMatrix(camera, cameraTransform, camera->m_NearPlane, shadowCascadeLevels[i], lightDirection));
		else if(i < shadowCascadeLevels.size())
			matrices.push_back(GenerateLightSpaceMatrix(camera, cameraTransform, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], lightDirection));
		else
			matrices.push_back(GenerateLightSpaceMatrix(camera, cameraTransform, shadowCascadeLevels[i - 1], camera->m_FarPlane, lightDirection));
	}

	return matrices;
}

glm::mat4 LightManager::GenerateLightSpaceMatrix(Camera* camera, Transform* cameraTransform, const float& nearPlane, const float& farPlane, const glm::vec3& lightDirection)
{
	std::vector<glm::vec4> corners = GetFrustumCornersWorldSpace(camera, cameraTransform, nearPlane, farPlane);
	glm::vec3 center = GetCenterOfPoints(corners);

	glm::mat4 lightView = glm::lookAt(center + glm::normalize(lightDirection), center, glm::vec3(0, 1.0f, 0.0f));
	glm::mat4 lightProjection = GetLightProjection(corners, lightView);

	return lightProjection * lightView;
}

std::vector<glm::vec4> LightManager::GetFrustumCornersWorldSpace(Camera* camera, Transform* cameraTransform, const float& nearPlane, const float& farPlane)
{
	glm::mat4 projection = glm::perspective(glm::radians(camera->m_Pov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 
		nearPlane, farPlane);
	glm::mat4 view = camera->GetView(cameraTransform);
	glm::mat4 matrix = projection * view;

	const auto inverse = glm::inverse(matrix);

	std::vector<glm::vec4> frustumCorners;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				glm::vec4 point = inverse * glm::vec4(
					2.0f * x - 1.0f,
					2.0f * y - 1.0f,
					2.0f * z - 1.0f,
					1.0f);
				frustumCorners.push_back(point / point.w);
			}
		}
	}
	return frustumCorners;
}

glm::vec3 LightManager::GetCenterOfPoints(const std::vector<glm::vec4>& corners)
{
	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const glm::vec4& point : corners)
		center +=  glm::vec3(point);
	center /= corners.size();
	return center;
}


glm::mat4 LightManager::GetLightProjection(std::vector<glm::vec4>& corners, const glm::mat4& lightView)
{
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const glm::vec4& v : corners)
	{
		const glm::vec4 trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	constexpr float zMult = 10;

	if(minZ < 0)
		minZ *= zMult;
	else
		minZ /= zMult;

	if(maxZ < 0)
		maxZ /= zMult;
	else
		maxZ *= zMult;

	return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
}