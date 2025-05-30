#include "AssetLoader.h"

void AssetLoader::LoadMesh(Mesh& mesh, const char* filepath)
{
	std::ifstream file(filepath); std::string line;
	if (!file) {
		std::cerr << "Cannot open file: " << filepath << std::endl;
	}

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if(prefix == "o")
			ss >> mesh.name;

		else if (prefix == "v") 
		{
			mesh.vertexPositions.push_back(glm::vec3());

			ss >> mesh.vertexPositions[mesh.vertexPositions.size() - 1].x;
			ss >> mesh.vertexPositions[mesh.vertexPositions.size() - 1].y;
			ss >> mesh.vertexPositions[mesh.vertexPositions.size() - 1].z;
		}
		else if (prefix == "vn")
		{
			mesh.vertexNormals.push_back(glm::vec3());

			ss >> mesh.vertexNormals[mesh.vertexNormals.size() - 1].x;
			ss >> mesh.vertexNormals[mesh.vertexNormals.size() - 1].y;
			ss >> mesh.vertexNormals[mesh.vertexNormals.size() - 1].z;
		}
		else if (prefix == "vt")
		{
			mesh.vertexTextureCoords.push_back(glm::vec2());

			ss >> mesh.vertexTextureCoords[mesh.vertexTextureCoords.size() - 1].x;
			ss >> mesh.vertexTextureCoords[mesh.vertexTextureCoords.size() - 1].y;
		}

		else if (prefix == "f")
		{

		}
	}

	std::cout << mesh.name << std::endl;
}
