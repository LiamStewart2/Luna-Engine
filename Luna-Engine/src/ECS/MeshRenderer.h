#pragma once

#include "Component.h"

#include "../Renderer.h"

class Mesh;
class Texture;
class Material;
class Shader;

class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* _gameObject, Mesh* _mesh, Texture* _texture, Material* _material, Shader* _shader) : 
		Component(_gameObject), mesh(_mesh), texture(_texture), material(_material), shader(_shader) {}

	Texture* texture;
	Mesh* mesh;
	Material* material;
	Shader* shader;

	void OnRender(Renderer* renderer) override
	{
		
	}
};