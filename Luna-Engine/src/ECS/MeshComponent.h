#pragma once

#include "Component.h"
#include <memory>

#include "../Renderer/ITexture.h"
#include "../Renderer/IMesh.h"
#include "../Renderer/IShader.h"
#include "../Renderer/Material.h"

struct MeshComponent : public Component
{
	MeshComponent(unsigned int _gameObject = 0, Luna::IMesh* _mesh = nullptr, Luna::IShader* _shader = nullptr, std::shared_ptr<Luna::Material> _material = nullptr) :
		Component(_gameObject), mesh(_mesh), shader(_shader), material(_material) {}
	Luna::IMesh* mesh;
	Luna::IShader* shader;
	std::shared_ptr<Luna::Material> material;
};