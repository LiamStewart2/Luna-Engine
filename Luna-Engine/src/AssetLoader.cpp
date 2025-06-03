#include "AssetLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

void AssetLoader::LoadMeshOBJ(Mesh& mesh, const char* filepath)
{
	double startTime = glfwGetTime();

	std::ifstream file(filepath); std::string line;
	if (!file) {
		std::cerr << "Cannot open file: " << filepath << std::endl;
		return;
	}

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec2> vertexTextureCoords;

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string prefix;
		ss >> prefix;


		if(prefix == "o")
			ss >> mesh.name;

		else if (prefix == "v") 
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
				vertex.TextureCoordiante = vertexTextureCoords[vertexTextureCoordinateIndex];
				vertex.Normal = vertexNormals[vertexNormalIndex];

				int vertexIndex = -1;
				for (int i = 0; i < mesh.vertices.size(); i++)
				{
					if (vertex.Position == mesh.vertices[i].Position && vertex.TextureCoordiante == mesh.vertices[i].TextureCoordiante && vertex.Normal == mesh.vertices[i].Normal)
					{
						vertexIndex = i;
						break;
					}
				}

				if (vertexIndex == -1)
				{
					mesh.vertices.push_back(vertex);
					mesh.indices.push_back(mesh.vertices.size() - 1);
				}
				else
					mesh.indices.push_back(vertexIndex);
			}
		}
	}
	mesh.BuildMesh();

	std::cout << "Mesh Loaded - " << mesh.name << " - Time Took: " << glfwGetTime() - startTime << std::endl;
}


void AssetLoader::LoadTexture(Texture& texture, const char* filepath)
{
	double startTime = glfwGetTime();

	unsigned char* data = stbi_load(filepath, &texture.width, &texture.height, &texture.channels, 0);

	if (data)
		texture.BuildTexture(data);
	else
		std::cerr << "Failed to load texture: " << filepath << std::endl;
	stbi_image_free(data);

	std::cout << "Texture Loaded - " << filepath << " - Time Took: " << glfwGetTime() - startTime << std::endl;
}