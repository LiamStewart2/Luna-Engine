// Luna Engine - Asset Loader

/* 
Collection of file loading methods for various object types currently including
 - OBJ to mesh loading
 - stb_image supported files loading to texture
*/

#include "AssetLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include <chrono>
#include <string.h>

// void LoadMeshOBJ - Loads a mesh object from an OBJ file
// OBJ files must be triangulated and only contain one mesh
// std::shared_ptr<Mesh> mesh - a reference to the mesh object the data should be loaded into
// const char* filepath - the file path of the .obj file. currently does not check if file extension is .obj
namespace Luna
{
	ConcurrentQueue<MeshData*> AssetLoader::s_MeshQueue = ConcurrentQueue<MeshData*>();
	std::vector<std::thread> AssetLoader::s_WorkerThreads = std::vector<std::thread>();
	inline int FastAtoi3(char*& p)
	{
		int value = (p[0] - '0');
		p++;

		while (*p >= '0')
		{
			value = value * 10 + (*p - '0');
			++p;
		}

		return value;
	}

	void AssetLoader::LoadMeshOBJAsynchronous(std::shared_ptr<MeshHandle> mesh, std::string filepath)
	{
		MeshData* meshData = new MeshData();

		meshData->m_MeshHandle = mesh;
		meshData->m_filepath = filepath;
		mesh->m_HandleState = AssetHandleState::Loading;

		s_WorkerThreads.emplace_back(LoadMeshFromOBJ, meshData);
	}

	void AssetLoader::CleanupThreads()
	{
		for (int i = 0; i < s_WorkerThreads.size(); i++)
		{
			s_WorkerThreads[i].join();
		}
		s_WorkerThreads.clear();
	}

	void AssetLoader::CreateMeshesFromQueue()
	{
		while (!s_MeshQueue.isEmpty())
		{
			MeshData* mesh = s_MeshQueue.Pop();
			
			mesh->m_MeshHandle->m_MeshReference = IMesh::Create(mesh->m_VertexBuffer, mesh->m_IndexBuffer);
			mesh->m_MeshHandle->m_MeshReference->m_Path = std::string(mesh->m_filepath);
			mesh->m_MeshHandle->m_HandleState = AssetHandleState::Loaded;

			delete mesh;
		}
	}

	void AssetLoader::LoadMeshOBJ(std::shared_ptr<MeshHandle>& mesh, std::string filepath)
	{
		MeshData* meshData = new MeshData();

		meshData->m_MeshHandle = mesh;
		meshData->m_filepath = filepath;
		mesh->m_HandleState = AssetHandleState::Loading;

		LoadMeshFromOBJ(meshData);
	}

	void AssetLoader::LoadMeshFromOBJ(MeshData* mesh)
	{
		const auto start{ std::chrono::steady_clock::now() };
		// stop the process if filepath is invalid
		// this wont crash anything, but will result in the mesh buffers being empty - nothing will render


		FILE* fptr;
		fopen_s(&fptr, mesh->m_filepath.c_str(), "r");

		if (fptr == NULL)
		{
			std::cerr << "Cannot open file: " << mesh->m_filepath << std::endl;
			return;
		}

		char lineBuffer[200];

		// allocate arrays and maps
		unsigned int vertexPositionsCounter = 0;
		unsigned int vertexNormalsCounter = 0;
		unsigned int vertexTextureCoordCounter = 0;
		unsigned int indexCounter = 0;
		unsigned int vertexCounter = 0;

		glm::vec3* vertexPositionBuffer;
		glm::vec3* vertexNormalBuffer;
		glm::vec2* vertexTextureCoordBuffer;

		unsigned int* indexBuffer;
		Vertex* vertexBuffer;

		vertexPositionBuffer = new glm::vec3[s_BufferSize];
		vertexNormalBuffer = new glm::vec3[s_BufferSize];
		vertexTextureCoordBuffer = new glm::vec2[s_BufferSize];

		indexBuffer = new unsigned int[s_BufferSize];
		vertexBuffer = new Vertex[s_BufferSize];

		Vertex currentVertex;
		int positionIndex;
		int normalIndex;
		int textureCoordIndex;

		glm::vec3 vertexPos;
		glm::vec3 vertexNorm;
		glm::vec2 vertexTexCoord;

		char* ending;

		bool firstTime = false;

		// Go through all lines again, parsing data
		while (fgets(lineBuffer, 200, fptr))
		{
			if (lineBuffer[0] == 'v')
			{
				if (lineBuffer[1] == ' ')
				{
					vertexPos.x = strtof(lineBuffer + 2, &ending);
					vertexPos.y = strtof(ending, &ending);
					vertexPos.z = strtof(ending, NULL);

					vertexPositionBuffer[vertexPositionsCounter] = vertexPos;
					vertexPositionsCounter++;
				}
				else if (lineBuffer[1] == 'n')
				{
					vertexNorm.x = strtof(lineBuffer + 3, &ending);
					vertexNorm.y = strtof(ending, &ending);
					vertexNorm.z = strtof(ending, NULL);

					vertexNormalBuffer[vertexNormalsCounter] = vertexNorm;
					vertexNormalsCounter++;
				}
				else if (lineBuffer[1] == 't')
				{
					vertexTexCoord.x = strtof(lineBuffer + 3, &ending);
					vertexTexCoord.y = strtof(ending, NULL);

					vertexTextureCoordBuffer[vertexTextureCoordCounter] = vertexTexCoord;
					vertexTextureCoordCounter++;
				}
			}
			else if (lineBuffer[0] == 'f')
			{
				if (lineBuffer[1] == ' ')
				{
					ending = lineBuffer + 2;

					for (int i = 0; i < 3; i++)
					{
						positionIndex = FastAtoi3(ending) - 1; ending++;
						textureCoordIndex = FastAtoi3(ending) - 1; ending++;
						normalIndex = FastAtoi3(ending) - 1; if (*ending == ' ') ending++;

						currentVertex.Position = vertexPositionBuffer[positionIndex];
						currentVertex.TextureCoordinate = vertexTextureCoordBuffer[textureCoordIndex];
						currentVertex.Normal = vertexNormalBuffer[normalIndex];

						indexBuffer[indexCounter] = vertexCounter;
						vertexBuffer[vertexCounter] = currentVertex;

						indexCounter++;
						vertexCounter++;
					}


				}
			}
		}

		fclose(fptr);

		unsigned int i0 = 0;
		unsigned int i1 = 0;
		unsigned int i2 = 0;
		glm::vec3 v, w;
		float sx, sy, tx, ty, dirCorrection;
		glm::vec3 tangent, bitangent;
		unsigned int tangentIndex;
		glm::vec3 localTangent, localBitangent;

		for (unsigned int i = 0; i < indexCounter; i += 3)
		{
			i0 = indexBuffer[i + 0]; i1 = indexBuffer[i + 1]; i2 = indexBuffer[i + 2];
			v = vertexBuffer[i1].Position - vertexBuffer[i0].Position;
			w = vertexBuffer[i2].Position - vertexBuffer[i0].Position;

			sx = vertexBuffer[i1].TextureCoordinate.x - vertexBuffer[i0].TextureCoordinate.x; sy = vertexBuffer[i1].TextureCoordinate.y - vertexBuffer[i0].TextureCoordinate.y;
			tx = vertexBuffer[i2].TextureCoordinate.x - vertexBuffer[i0].TextureCoordinate.x; ty = vertexBuffer[i2].TextureCoordinate.y - vertexBuffer[i0].TextureCoordinate.y;
			dirCorrection = (tx * sy - ty * sx) < 0.0f ? -1.0f : 1.0f;

			if (sx * ty == sy * tx)
			{
				sx = 0.0f;
				sy = 1.0f;
				tx = 1.0f;
				ty = 0.0f;
			}

			tangent = (w * sy - v * ty) * dirCorrection;
			bitangent = (w * sx - v * tx) * dirCorrection;

			for (int j = 0; j < 3; j++)
			{
				tangentIndex = indexBuffer[i + j];

				localTangent = tangent - vertexBuffer[tangentIndex].Normal * (tangent * vertexBuffer[tangentIndex].Normal);
				localBitangent = bitangent - vertexBuffer[tangentIndex].Normal * (bitangent * vertexBuffer[tangentIndex].Normal);

				vertexBuffer[tangentIndex].Tangent = glm::normalize(localTangent);
				vertexBuffer[tangentIndex].Bitangent = glm::normalize(localBitangent);
			}
		}

		mesh->m_VertexBuffer = std::vector<Vertex>(vertexBuffer, vertexBuffer + vertexCounter);
		mesh->m_IndexBuffer = std::vector<unsigned int>(indexBuffer, indexBuffer + indexCounter);

		const auto finish{ std::chrono::steady_clock::now() };
		const auto elapsed_seconds_finished = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
		std::cout << "Mesh Loaded - " << mesh->m_filepath << " - Time to Load: " << elapsed_seconds_finished << " Vertices: " << vertexCounter << std::endl;
	
		s_MeshQueue.Push(mesh);

		delete[] vertexPositionBuffer;
		delete[] vertexNormalBuffer;
		delete[] vertexTextureCoordBuffer;

		delete[] indexBuffer;
		delete[] vertexBuffer;
	}


	// void LoadTexture - Loads a texture from any stb_image supported file
	// Supports both RGB and RGBA channelled images
	// std::shared_ptr<Texture> texture - a reference to the texture object the data should be loaded into
	// const char* filepath - the file path of the image file
	void AssetLoader::LoadTexture(std::shared_ptr<TextureHandle> texture, const char* filepath)
	{
		texture->m_HandleState = AssetHandleState::Loading;
		double startTime = glfwGetTime();
		
		TexturePacket packet;
		packet.path = filepath;
		packet.buffer = stbi_load(packet.path.c_str(), &packet.width, &packet.height, &packet.channels, STBI_rgb_alpha);
		if (!packet.buffer)
		{
			std::cerr << "Failed to load texture - " << packet.path << std::endl;
			return;
		}
		texture->m_TextureReference = ITexture::Create(packet);
		stbi_image_free(packet.buffer);

		std::cout << "Texture Loaded - " << filepath << " - Time Took: " << glfwGetTime() - startTime << std::endl;
		texture->m_HandleState = AssetHandleState::Loaded;
	}
}