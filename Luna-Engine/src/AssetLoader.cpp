#include "AssetLoader.h"

#include <GLFW/glfw3.h>

void AssetLoader::LoadMeshOBJ(Mesh& mesh, const char* filepath)
{
	double startTime = glfwGetTime();

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
			Face face; std::string indicies; Vertex vertex;
			mesh.faces.push_back(face);

			for(int i = 0; i < 3; i++)
			{
				ss >> indicies;
				vertex.positionIndex = std::stoi(indicies.substr(0, indicies.find("/")));
				vertex.textureCoordinateIndex = std::stoi(indicies.substr(indicies.find("/") + 1, indicies.rfind("/")));
				vertex.normalIndex = std::stoi(indicies.substr(indicies.rfind("/") + 1, indicies.size()));

				mesh.faces[mesh.faces.size() - 1].vertices.push_back(vertex);
			}
		}
	}
	std::cout << "Mesh Loaded - " << mesh.name << " - Time Took: " << glfwGetTime() - startTime << std::endl;

}


