// Luna Engine - Asset Loader

#pragma once

#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include "JSON/json.hpp"

#include "../Renderer/ITexture.h"
#include "../Renderer/IMesh.h"
#include "../Renderer/Material.h"
#include "../Core/ConcurrentQueue.h"
#include "../ECS/MeshComponent.h"

/*
Collection of file loading methods for various object types currently including
 - OBJ to mesh loading
 - stb_image supported files loading to texture
*/
namespace Luna
{
	struct MeshData
	{
		std::shared_ptr<MeshHandle> m_MeshHandle;
		std::string m_filepath;
		std::vector<Vertex> m_VertexBuffer;
		std::vector<unsigned int> m_IndexBuffer;
	};

	class AssetLoader
	{
	public:
		// void LoadTexture - Loads a texture from any stb_image supported file
		// Supports both RGB and RGBA channelled images
		// std::shared_ptr<Texture> texture - a reference to the texture object the data should be loaded into
		// const char* filepath - the file path of the image file
		static void LoadTexture(std::shared_ptr<TextureHandle> texture, const char* filepath);

		// void LoadMeshOBJ - Loads a mesh object from an OBJ file
		// OBJ files must be triangulated and only contain one mesh
		// std::shared_ptr<Mesh> mesh - a reference to the mesh object the data should be loaded into
		// const char* filepath - the file path of the .obj file. currently does not check if file extension is .obj
		static void LoadMeshOBJ(std::shared_ptr<MeshHandle>& mesh, std::string filepath);

		static void LoadMeshOBJAsynchronous(std::shared_ptr<MeshHandle> mesh, std::string filepath);
		static void CreateMeshesFromQueue();
		static void CleanupThreads();

	private:
		// Buffers for loading Meshes
		static const unsigned int s_BufferSize = 1200000;
		
		static void LoadMeshFromOBJ(MeshData* mesh);

		static ConcurrentQueue<MeshData*> s_MeshQueue;
		static std::vector<std::thread> s_WorkerThreads;
	};
}

