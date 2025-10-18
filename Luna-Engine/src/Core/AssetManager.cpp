// Luna Engine - AssetManager

/*
	Uses Luna Engine - AssetLoader to load and manage buffers of all assets used in the lifetime of an application

	ToDo: For each asset type, check the file extension and use corresponding functions to make loading assets smoother
*/

#include "AssetManager.h"
namespace Luna
{
	AssetManager::AssetManager()
	{
	}

	AssetManager::~AssetManager()
	{
	}

	// std::shared_ptr<Texture> GetTexture - Loads a texture from filepath and returns a reference to the object from the buffer
	// std::string filepath - the file path of the image file

	std::shared_ptr<ITexture> AssetManager::GetTexture(std::string filepath)
	{
		if (m_Textures.find(filepath) == m_Textures.end())
		{

			std::cout << filepath << std::endl;
			std::shared_ptr<ITexture> texture = m_Textures[filepath] = std::make_shared<ITexture>();
			AssetLoader::LoadTexture(texture, filepath.c_str());
		}
		return m_Textures[filepath];
	}

	// std::shared_ptr<Mesh> GetMesh - Loads a mesh from filepath and returns a reference to the object from the buffer
	// std::string filepath - the file path of the mesh file

	std::shared_ptr<IMesh> AssetManager::GetMesh(std::string filepath)
	{
		if (m_Meshes.find(filepath) == m_Meshes.end())
		{

			std::cout << filepath << std::endl;
			std::shared_ptr<IMesh> mesh = m_Meshes[filepath] = std::make_shared<IMesh>();
			AssetLoader::LoadMeshOBJ(mesh, filepath.c_str());
		}
		return m_Meshes[filepath];
	}

	// std::shared_ptr<Shader> GetTexture - Loads a shader from folder and returns a reference to the object from the buffer
	// std::string filepath - the file path of the shader folder

	std::shared_ptr<IShader> AssetManager::GetShader(std::string filepath)
	{
		if (m_Shaders.find(filepath) == m_Shaders.end())
		{
			std::cout << filepath << std::endl;
			m_Shaders[filepath] = IShader::Create(filepath);
		}
		return m_Shaders[filepath];
	}

	std::shared_ptr<Script> AssetManager::GetScript(std::string filepath)
	{
		if (m_Scripts.find(filepath) == m_Scripts.end())
		{
			std::cout << filepath << std::endl;
			std::shared_ptr<Script> script = m_Scripts[filepath] = std::make_shared<Script>(filepath);

		}
		return m_Scripts[filepath];
	}
}
