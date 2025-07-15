#pragma once

#include "GLM/glm.hpp"

#include "SceneGraph.h"
#include "ECS/ECS.h"

class TransformationManager
{
public:
	TransformationManager() {}
	~TransformationManager() {}

	void UpdateTransformationMatricies(SceneGraph* sceneGraph, EntityComponentSystem* ECS);

private:
	void UpdateTransform();
};