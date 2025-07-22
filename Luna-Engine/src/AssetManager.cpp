#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

std::shared_ptr<Texture> AssetManager::GetTexture(std::string filepath)
{
	if(m_Textures.find(filepath) == m_Textures.end())
	{
		
		std::shared_ptr<Texture> texture = m_Textures[filepath] = std::make_shared<Texture>();
		AssetLoader::LoadTexture(texture, filepath.c_str());
	}
	return m_Textures[filepath];
}

std::shared_ptr<Mesh> AssetManager::GetMesh(std::string filepath)
{
	if (m_Meshes.find(filepath) == m_Meshes.end())
	{

		std::shared_ptr<Mesh> mesh = m_Meshes[filepath] = std::make_shared<Mesh>();
		AssetLoader::LoadMeshOBJ(mesh, filepath.c_str());
	}
	return m_Meshes[filepath];
}

std::shared_ptr<Shader> AssetManager::GetShader(std::string filepath)
{
	if (m_Shaders.find(filepath) == m_Shaders.end())
		m_Shaders[filepath] = std::make_shared<Shader>(filepath + std::string("/shader.vs"), filepath + std::string("/shader.fs"), filepath + std::string("/shader.gs"));
	return m_Shaders[filepath];
}
