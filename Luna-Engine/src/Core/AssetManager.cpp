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

		std::cout << filepath << std::endl;
		std::shared_ptr<Texture> texture = m_Textures[filepath] = std::make_shared<Texture>();
		AssetLoader::LoadTexture(texture, filepath.c_str());
	}
	return m_Textures[filepath];
}

std::shared_ptr<Mesh> AssetManager::GetMesh(std::string filepath)
{
	if (m_Meshes.find(filepath) == m_Meshes.end())
	{

		std::cout << filepath << std::endl;
		std::shared_ptr<Mesh> mesh = m_Meshes[filepath] = std::make_shared<Mesh>();
		AssetLoader::LoadMeshOBJ(mesh, filepath.c_str());
	}
	return m_Meshes[filepath];
}

std::shared_ptr<Shader> AssetManager::GetShader(std::string filepath)
{
	if (m_Shaders.find(filepath) == m_Shaders.end())
	{
		std::cout << filepath << std::endl;
		m_Shaders[filepath] = std::make_shared<Shader>(filepath.c_str(), (filepath + std::string("/shader.vs")).c_str(), (filepath + std::string("/shader.fs")).c_str(), (filepath + std::string("/shader.gs")).c_str());
	}
	return m_Shaders[filepath];
}
