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
	void AssetLoader::LoadMeshOBJ(std::shared_ptr<IMesh>& mesh, const char* filepath)
	{
		const auto start{std::chrono::steady_clock::now()};
		// stop the process if filepath is invalid
		// this wont crash anything, but will result in the mesh buffers being empty - nothing will render
		if(false)
		{
			std::ifstream file(filepath); std::string line;
			if (!file) {
				std::cerr << "Cannot open file: " << filepath << std::endl;
				return;
			}

			std::unordered_map<Vertex, int, Vertex::Hash> vertexMap;
			std::vector<Vertex> vertices;

			std::vector<unsigned int> indices = std::vector<unsigned int>();

			// Create buffers for the mesh vertex data
			std::vector<glm::vec3> vertexPositions;
			std::vector<glm::vec3> vertexNormals;
			std::vector<glm::vec2> vertexTextureCoords;

			while (std::getline(file, line))
			{
				std::istringstream ss(line);
				std::string prefix;
				ss >> prefix;

				if (prefix == "v")
				{
					vertexPositions.emplace_back(glm::vec3());

					ss >> vertexPositions[vertexPositions.size() - 1].x;
					ss >> vertexPositions[vertexPositions.size() - 1].y;
					ss >> vertexPositions[vertexPositions.size() - 1].z;
				}
				else if (prefix == "vn")
				{
					vertexNormals.emplace_back(glm::vec3());

					ss >> vertexNormals[vertexNormals.size() - 1].x;
					ss >> vertexNormals[vertexNormals.size() - 1].y;
					ss >> vertexNormals[vertexNormals.size() - 1].z;
				}
				else if (prefix == "vt")
				{
					vertexTextureCoords.emplace_back(glm::vec2());

					ss >> vertexTextureCoords[vertexTextureCoords.size() - 1].x;
					ss >> vertexTextureCoords[vertexTextureCoords.size() - 1].y;
				}

				// Generate the face data, combining indices together for the meshes IBO
				else if (prefix == "f")
				{
					std::string indicies; Vertex vertex;

					for (int i = 0; i < 3; i++)
					{
						//string representing vertice
						ss >> indicies;

						int vertexPositionIndex = std::stoi(indicies.substr(0, indicies.find("/"))) - 1;
						int vertexTextureCoordinateIndex = std::stoi(indicies.substr(indicies.find("/") + 1, indicies.rfind("/"))) - 1;
						int vertexNormalIndex = std::stoi(indicies.substr(indicies.rfind("/") + 1, indicies.size())) - 1;

						if (vertexPositionIndex >= vertexPositions.size() || vertexTextureCoordinateIndex >= vertexTextureCoords.size() || vertexNormalIndex >= vertexNormals.size())
							break;

						vertex.Position = vertexPositions[vertexPositionIndex];
						vertex.TextureCoordinate = vertexTextureCoords[vertexTextureCoordinateIndex];
						vertex.Normal = vertexNormals[vertexNormalIndex];

						auto it = vertexMap.find(vertex);
						if (it == vertexMap.end())
						{
							int index = vertices.size();
							indices.emplace_back(index);
							vertices.emplace_back(vertex);
							vertexMap[vertex] = index;
						}
						else
						{
							indices.emplace_back(it->second);
						}

					}
				}
			}

			// Calculate tangents and bitangents

			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				unsigned int i0 = indices[i + 0], i1 = indices[i + 1], i2 = indices[i + 2];
				glm::vec3 v = vertices[i1].Position - vertices[i0].Position, w = vertices[i2].Position - vertices[i0].Position;

				float sx = vertices[i1].TextureCoordinate.x - vertices[i0].TextureCoordinate.x, sy = vertices[i1].TextureCoordinate.y - vertices[i0].TextureCoordinate.y;
				float tx = vertices[i2].TextureCoordinate.x - vertices[i0].TextureCoordinate.x, ty = vertices[i2].TextureCoordinate.y - vertices[i0].TextureCoordinate.y;
				float dirCorrection = (tx * sy - ty * sx) < 0.0f ? -1.0f : 1.0f;

				if (sx * ty == sy * tx)
				{
					sx = 0.0f;
					sy = 1.0f;
					tx = 1.0f;
					ty = 0.0f;
				}

				glm::vec3 tangent, bitangent;
				tangent = (w * sy - v * ty) * dirCorrection;
				bitangent = (w * sx - v * tx) * dirCorrection;

				for (int j = 0; j < 3; j++)
				{
					unsigned int index = indices[i + j];

					glm::vec3 localTangent = tangent - vertices[index].Normal * (tangent * vertices[index].Normal);
					glm::vec3 localBitangent = bitangent - vertices[index].Normal * (bitangent * vertices[index].Normal);

					vertices[index].Tangent = glm::normalize(localTangent);
					vertices[index].Bitangent = glm::normalize(localBitangent);
				}
			}

			mesh = IMesh::Create(vertices, indices);
			mesh->m_Path = std::string(filepath);
		}
		
		else
		{

			FILE* fptr;
			fopen_s(&fptr, filepath, "r");

			if (fptr == NULL)
			{
				std::cerr << "Cannot open file: " << filepath << std::endl;
				return;
			}

			char lineBuffer[100];

			unsigned int vertexCount = 0;
			unsigned int indexCount = 0;
			unsigned int vertexPositionCount = 0;
			unsigned int vertexNormalCount = 0;
			unsigned int vertexTextureCoordCount = 0;

			// iterate through all lines
			while (fgets(lineBuffer, 100, fptr))
			{
				switch (lineBuffer[0])
				{
				case('v'):
					if (lineBuffer[1] == ' ')
						vertexPositionCount += 1;
					else if (lineBuffer[1] == 'n')
						vertexNormalCount += 1;
					else if (lineBuffer[1] == 't')
						vertexTextureCoordCount += 1;
					break;
				case('f'):
					if (lineBuffer[1] == ' ')
						indexCount += 3;
					break;

				}
			}


			// Reset file navigation
			rewind(fptr);

			// allocate arrays and maps

			std::unordered_map<Vertex, int, Vertex::Hash> vertexMap;
			glm::vec3* vertexPositions = new glm::vec3[vertexPositionCount];
			glm::vec3* vertexNormals = new glm::vec3[vertexNormalCount];
			glm::vec2* vertexTextureCoords = new glm::vec2[vertexTextureCoordCount];

			std::vector<unsigned int> indices = std::vector<unsigned int>(); indices.resize(indexCount);
			std::vector<Vertex> vertices = std::vector<Vertex>(); vertices.resize(indexCount);

			unsigned int vertexPositionsCounter = 0;
			unsigned int vertexNormalsCounter = 0;
			unsigned int vertexTextureCoordCounter = 0;
			unsigned int indexCounter = 0; 
			unsigned int vertexCounter = 0;
			// Go through all lines again, parsing data
			while (fgets(lineBuffer, 100, fptr))
			{
				switch (lineBuffer[0])
				{
				case('v'):
					if (lineBuffer[1] == ' ')
					{
						glm::vec3 vertPos = glm::vec3(); char* ending;
						vertPos.x = strtof(lineBuffer + 2, &ending);
						vertPos.y = strtof(ending, &ending);
						vertPos.z = strtof(ending, NULL);

						vertexPositions[vertexPositionsCounter] = vertPos;
						vertexPositionsCounter += 1;
					}
					else if (lineBuffer[1] == 'n')
					{
						glm::vec3 vertNorm = glm::vec3(); char* ending;
						vertNorm.x = strtof(lineBuffer + 3, &ending);
						vertNorm.y = strtof(ending, &ending);
						vertNorm.z = strtof(ending, NULL);

						vertexNormals[vertexNormalsCounter] = vertNorm;
						vertexNormalsCounter += 1;
					}
					else if (lineBuffer[1] == 't')
					{
						glm::vec2 vertTexCoord = glm::vec2(); char* ending;
						vertTexCoord.x = strtof(lineBuffer + 3, &ending);
						vertTexCoord.y = strtof(ending, NULL);

						vertexTextureCoords[vertexTextureCoordCounter] = vertTexCoord;
						vertexTextureCoordCounter += 1;
					}
					break;
				case('f'):
					if (lineBuffer[1] == ' ')
					{

						char* ending = lineBuffer + 2;

						for(int i = 0; i < 3; i++)


						{
							Vertex vertex = Vertex();
							vertex.Position = vertexPositions[((int)strtof(ending, &ending)) - 1];
							vertex.TextureCoordinate = vertexTextureCoords[((int)strtof(ending + 1, &ending)) - 1];
							vertex.Normal = vertexNormals[((int)strtof(ending + 1, &ending)) - 1];

							auto it = vertexMap.find(vertex);
							if (it == vertexMap.end())
							{
								int vertexIndex = vertexCounter;

								indices[indexCounter] = vertexIndex;
								vertices[vertexIndex] = vertex;

								vertexMap[vertex] = vertexIndex;

								indexCounter += 1;
								vertexCounter += 1;
							}
							else
							{
								indices[indexCounter] = it->second;
								indexCounter += 1;
							}
						}


					}
					break;

				}
			}

			fclose(fptr);


			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				unsigned int i0 = indices[i + 0], i1 = indices[i + 1], i2 = indices[i + 2];
				glm::vec3 v = vertices[i1].Position - vertices[i0].Position;
				glm::vec3 w = vertices[i2].Position - vertices[i0].Position;

				float sx = vertices[i1].TextureCoordinate.x - vertices[i0].TextureCoordinate.x, sy = vertices[i1].TextureCoordinate.y - vertices[i0].TextureCoordinate.y;
				float tx = vertices[i2].TextureCoordinate.x - vertices[i0].TextureCoordinate.x, ty = vertices[i2].TextureCoordinate.y - vertices[i0].TextureCoordinate.y;
				float dirCorrection = (tx * sy - ty * sx) < 0.0f ? -1.0f : 1.0f;

				if (sx * ty == sy * tx)
				{
					sx = 0.0f;
					sy = 1.0f;
					tx = 1.0f;
					ty = 0.0f;
				}

				glm::vec3 tangent, bitangent;
				tangent = (w * sy - v * ty) * dirCorrection;
				bitangent = (w * sx - v * tx) * dirCorrection;

				for (int j = 0; j < 3; j++)
				{
					unsigned int index = indices[i + j];

					glm::vec3 localTangent = tangent - vertices[index].Normal * (tangent * vertices[index].Normal);
					glm::vec3 localBitangent = bitangent - vertices[index].Normal * (bitangent * vertices[index].Normal);

					vertices[index].Tangent = glm::normalize(localTangent);
					vertices[index].Bitangent = glm::normalize(localBitangent);
				}
			}


			mesh = IMesh::Create(vertices, indices);
			mesh->m_Path = std::string(filepath);

			const auto finish{ std::chrono::steady_clock::now() };
			const std::chrono::duration<double> elapsed_seconds{ finish - start };
			std::cout << "Mesh Loaded - " << mesh->m_Path << " - Time Took: " << elapsed_seconds << std::endl;

			delete[] vertexPositions;
			delete[] vertexNormals;
			delete[] vertexTextureCoords;
		}
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