#include "AssetLoader.h"

void AssetLoader::LoadMesh(Mesh& mesh, const char* filepath)
{
	std::ifstream file(filepath); std::string line;

	Vertex currentValues = Vertex();

	while (std::getline(file, line))
	{
		// Mesh Name
		if (line[0] == 'o')
			mesh.name =  line.substr(2, line.size() - 2);

		if (line[0] == 'v')
		{
			// Vertex Position
			if (line[1] == ' ')
			{
				int current = 1;
				for (int i = 0; i < 3; i++)
				{
					//std::cout <<  "current at start: " << current << ", ";
					std::size_t size = line.substr(current, line.size() - current).find(" ");
					std::cout << line.substr(current, line.size() - current) << "<- substr, ";
					float value = std::stof(line.substr(current, size - 1));
					currentValues.position[i] = value;
					current += size - 1;
					std::cout << "current after values: " << current << ", ";
					std::cout << "size: " << size << "\n";
				}
				std::cout << " : " << currentValues.position.x << ", " << currentValues.position.y << ", " << currentValues.position.z << "\n";
			}
		}
	}

	std::cout << mesh.name << std::endl;
}
