// Luna Engine - Asset Loader

/* 
Collection of file loading methods for various object types currently including
 - OBJ to mesh loading
 - stb_image supported files loading to texture
*/

#include "AssetLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

// void LoadMeshOBJ - Loads a mesh object from an OBJ file
// OBJ files must be triangulated and only contain one mesh
// std::shared_ptr<Mesh> mesh - a reference to the mesh object the data should be loaded into
// const char* filepath - the file path of the .obj file. currently does not check if file extension is .obj
namespace Luna
{
	void AssetLoader::LoadMeshOBJ(std::shared_ptr<IMesh>& mesh, const char* filepath)
	{
		double startTime = glfwGetTime();

		// stop the process if filepath is invalid
		// this wont crash anything, but will result in the mesh buffers being empty - nothing will render
		std::ifstream file(filepath); std::string line;
		if (!file) {
			std::cerr << "Cannot open file: " << filepath << std::endl;
			return;
		}

		std::vector<Vertex> vertices = std::vector<Vertex>();
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
				vertexPositions.push_back(glm::vec3());

				ss >> vertexPositions[vertexPositions.size() - 1].x;
				ss >> vertexPositions[vertexPositions.size() - 1].y;
				ss >> vertexPositions[vertexPositions.size() - 1].z;
			}
			else if (prefix == "vn")
			{
				vertexNormals.push_back(glm::vec3());

				ss >> vertexNormals[vertexNormals.size() - 1].x;
				ss >> vertexNormals[vertexNormals.size() - 1].y;
				ss >> vertexNormals[vertexNormals.size() - 1].z;
			}
			else if (prefix == "vt")
			{
				vertexTextureCoords.push_back(glm::vec2());

				ss >> vertexTextureCoords[vertexTextureCoords.size() - 1].x;
				ss >> vertexTextureCoords[vertexTextureCoords.size() - 1].y;
			}

			// Generate the face data, combining indices together for the meshes IBO
			else if (prefix == "f")
			{
				std::string indicies; Vertex vertex;

				for(int i = 0; i < 3; i++)
				{
					//string representing vertice
					ss >> indicies;

					int vertexPositionIndex =  std::stoi(indicies.substr(0, indicies.find("/"))) - 1;
					int vertexTextureCoordinateIndex = std::stoi(indicies.substr(indicies.find("/") + 1, indicies.rfind("/"))) - 1;
					int vertexNormalIndex = std::stoi(indicies.substr(indicies.rfind("/") + 1, indicies.size())) - 1;

					if(vertexPositionIndex >= vertexPositions.size() || vertexTextureCoordinateIndex >= vertexTextureCoords.size() || vertexNormalIndex >= vertexNormals.size())
						break;

					vertex.Position = vertexPositions[vertexPositionIndex];
					vertex.TextureCoordinate = vertexTextureCoords[vertexTextureCoordinateIndex];
					vertex.Normal = vertexNormals[vertexNormalIndex];

					// TODO use a hash map to speed up the face building phase

					int vertexIndex = -1;
					for (int i = 0; i < vertices.size(); i++)
					{
						if (vertex.Position == vertices[i].Position && vertex.TextureCoordinate == vertices[i].TextureCoordinate && vertex.Normal == vertices[i].Normal)
						{
							vertexIndex = i;
							break;
						}
					}

					if (vertexIndex == -1)
					{
						vertices.push_back(vertex);
						indices.push_back(vertices.size() - 1);
					}
					else
						indices.push_back(vertexIndex);

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

			for(int j = 0; j < 3; j++)
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
		

		std::cout << "Mesh Loaded - " << mesh->m_Path << " - Time Took: " << glfwGetTime() - startTime << std::endl;
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