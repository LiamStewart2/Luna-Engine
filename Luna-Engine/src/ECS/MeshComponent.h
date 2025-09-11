#pragma once

#include "Component.h"

#include "../Renderer/Mesh.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Material.h"

struct MeshComponent : public Component
{
	MeshComponent(unsigned int _gameObject = 0, Mesh* _mesh = nullptr, Shader* _shader = nullptr, Material* _material = nullptr, Texture* _texture = nullptr) :
		Component(_gameObject), mesh(_mesh), shader(_shader), material(_material), texture(_texture)  {}
	Mesh* mesh;
	Shader* shader;
	Material* material;
	Texture* texture;
};