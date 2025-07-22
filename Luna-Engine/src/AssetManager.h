#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "AssetLoader.h"

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	void LoadTexture(std::string filepath);
	void LoadMesh(std::string filepath);
	void LoadShader(std::string filepath);

	std::shared_ptr<Texture> GetTexture(std::string filepath);
	std::shared_ptr<Mesh> GetMesh(std::string filepath);
	std::shared_ptr<Shader> GetShader(std::string filepath);

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Mesh;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shader;

};

