#pragma once

#include "Component.h"

class MeshRenderer : public Component
{
	MeshRenderer(GameObject* gameObject) : Component(gameObject) {}
};