#pragma once

#include "Component.h"

#include "../Renderer-2/ITexture.h"
#include "../Renderer-2/IMesh.h"
#include "../Renderer-2/IShader.h"
#include "../Renderer-2/Material.h"

struct MeshComponent : public Component
{
	MeshComponent(unsigned int _gameObject = 0, Luna::IMesh* _mesh = nullptr, Luna::IShader* _shader = nullptr, Luna::Material* _material = nullptr, Luna::ITexture* _texture = nullptr, Luna::ITexture* _specularMap = nullptr) :
		Component(_gameObject), mesh(_mesh), shader(_shader), material(_material), texture(_texture), specularMap(_specularMap) {}
	Luna::IMesh* mesh;
	Luna::IShader* shader;
	Luna::Material* material;
	Luna::ITexture* texture;
	Luna::ITexture* specularMap;
};