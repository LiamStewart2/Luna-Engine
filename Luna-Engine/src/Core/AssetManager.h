#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "AssetLoader.h"

#include "../Renderer/Texture.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Shader.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	std::shared_ptr<Texture> GetTexture(std::string filepath);
	std::shared_ptr<Mesh> GetMesh(std::string filepath);
	std::shared_ptr<Shader> GetShader(std::string filepath);

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};

