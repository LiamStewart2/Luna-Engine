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

	glm::vec3* AssetLoader::s_VertexPositionBuffer = nullptr;
	glm::vec3* AssetLoader::s_VertexNormalBuffer = nullptr;
	glm::vec2* AssetLoader::s_VertexTextureCoordBuffer = nullptr;

	unsigned int* AssetLoader::s_IndexBuffer = nullptr;
	Vertex* AssetLoader::s_VertexBuffer = nullptr;

	void AssetLoader::SetupBuffers()
	{
		s_VertexPositionBuffer = new glm::vec3[s_BufferSize];
		s_VertexNormalBuffer = new glm::vec3[s_BufferSize];
		s_VertexTextureCoordBuffer = new glm::vec2[s_BufferSize];

		s_IndexBuffer = new unsigned int[s_BufferSize];
		s_VertexBuffer = new Vertex[s_BufferSize];
	}

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

	void AssetLoader::LoadMeshOBJ(std::shared_ptr<IMesh>& mesh, const char* filepath)
	{
		const auto start{std::chrono::steady_clock::now()};
		// stop the process if filepath is invalid
		// this wont crash anything, but will result in the mesh buffers being empty - nothing will render


		FILE* fptr;
		fopen_s(&fptr, filepath, "r");

		if (fptr == NULL)
		{
			std::cerr << "Cannot open file: " << filepath << std::endl;
			return;
		}

		if(s_VertexPositionBuffer == nullptr)
			SetupBuffers();

		char lineBuffer[200];

		// allocate arrays and maps
		unsigned int vertexPositionsCounter = 0;
		unsigned int vertexNormalsCounter = 0;
		unsigned int vertexTextureCoordCounter = 0;
		unsigned int indexCounter = 0; 
		unsigned int vertexCounter = 0;

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
			if(lineBuffer[0] == 'v')
			{
				if (lineBuffer[1] == ' ')
				{
					vertexPos.x = strtof(lineBuffer + 2, &ending);
					vertexPos.y = strtof(ending, &ending);
					vertexPos.z = strtof(ending, NULL);

					s_VertexPositionBuffer[vertexPositionsCounter] = vertexPos;
					vertexPositionsCounter++;
				}
				else if (lineBuffer[1] == 'n')
				{
					vertexNorm.x = strtof(lineBuffer + 3, &ending);
					vertexNorm.y = strtof(ending, &ending);
					vertexNorm.z = strtof(ending, NULL);

					s_VertexNormalBuffer[vertexNormalsCounter] = vertexNorm;
					vertexNormalsCounter++;
				}
				else if (lineBuffer[1] == 't')
				{
					vertexTexCoord.x = strtof(lineBuffer + 3, &ending);
					vertexTexCoord.y = strtof(ending, NULL);

					s_VertexTextureCoordBuffer[vertexTextureCoordCounter] = vertexTexCoord;
					vertexTextureCoordCounter++;
				}
			}
			else if(lineBuffer[0] == 'f')
			{
				if (lineBuffer[1] == ' ')
				{
					ending = lineBuffer + 2;

					for (int i = 0; i < 3; i++)
					{
						positionIndex = FastAtoi3(ending) - 1; ending++;
						textureCoordIndex = FastAtoi3(ending) - 1; ending++;
						normalIndex = FastAtoi3(ending) - 1; if (*ending == ' ') ending++;

						currentVertex.Position = s_VertexPositionBuffer[positionIndex];
						currentVertex.TextureCoordinate = s_VertexTextureCoordBuffer[textureCoordIndex];
						currentVertex.Normal = s_VertexNormalBuffer[normalIndex];

						s_IndexBuffer[indexCounter] = vertexCounter;
						s_VertexBuffer[vertexCounter] = currentVertex;

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
			i0 = s_IndexBuffer[i + 0]; i1 = s_IndexBuffer[i + 1]; i2 = s_IndexBuffer[i + 2];
			v = s_VertexBuffer[i1].Position - s_VertexBuffer[i0].Position;
			w = s_VertexBuffer[i2].Position - s_VertexBuffer[i0].Position;

			sx = s_VertexBuffer[i1].TextureCoordinate.x - s_VertexBuffer[i0].TextureCoordinate.x; sy = s_VertexBuffer[i1].TextureCoordinate.y - s_VertexBuffer[i0].TextureCoordinate.y;
			tx = s_VertexBuffer[i2].TextureCoordinate.x - s_VertexBuffer[i0].TextureCoordinate.x; ty = s_VertexBuffer[i2].TextureCoordinate.y - s_VertexBuffer[i0].TextureCoordinate.y;
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
				tangentIndex = s_IndexBuffer[i + j];

				localTangent = tangent - s_VertexBuffer[tangentIndex].Normal * (tangent * s_VertexBuffer[tangentIndex].Normal);
				localBitangent = bitangent - s_VertexBuffer[tangentIndex].Normal * (bitangent * s_VertexBuffer[tangentIndex].Normal);

				s_VertexBuffer[tangentIndex].Tangent = glm::normalize(localTangent);
				s_VertexBuffer[tangentIndex].Bitangent = glm::normalize(localBitangent);
			}
		}

		std::vector<Vertex> verts(s_VertexBuffer, s_VertexBuffer + vertexCounter);
		std::vector<unsigned int> inds(s_IndexBuffer, s_IndexBuffer + indexCounter);


		mesh = IMesh::Create(verts, inds);
		mesh->m_Path = std::string(filepath);

		const auto finish{ std::chrono::steady_clock::now() };
		const auto elapsed_seconds_finished = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
		std::cout << "Mesh Loaded - " << mesh->m_Path << " - Time to Load: " << elapsed_seconds_finished << " Vertices: " << vertexCounter << std::endl;
	}


	// void LoadTexture - Loads a texture from any stb_image supported file
	// Supports both RGB and RGBA channelled images
	// std::shared_ptr<Texture> texture - a reference to the texture object the data should be loaded into
	// const char* filepath - the file path of the image file
	void AssetLoader::LoadTexture(std::shared_ptr<ITexture>& texture, const char* filepath)
	{
		double startTime = glfwGetTime();

		TexturePacket packet;
		packet.path = filepath;
		packet.buffer = stbi_load(packet.path.c_str(), &packet.width, &packet.height, &packet.channels, STBI_rgb_alpha);
		if (!packet.buffer)
		{
			std::cerr << "Failed to load texture - " << packet.path << std::endl;
			return;
		}
		texture = ITexture::Create(packet);
		stbi_image_free(packet.buffer);

		std::cout << "Texture Loaded - " << filepath << " - Time Took: " << glfwGetTime() - startTime << std::endl;
	}
}