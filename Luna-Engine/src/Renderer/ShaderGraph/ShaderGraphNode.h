#pragma once


#include <vector>
#include "GLM/glm.hpp"
#include "ShaderGraphNodeType.h"

namespace Luna
{
	class ShaderGraphNode;

	struct ShaderGraphConnection
	{
		ShaderGraphConnection(ShaderGraphNode* connection, int nodeIndex) 
			: m_Connection(connection), m_NodeIndex(nodeIndex) {}

		ShaderGraphNode* m_Connection;
		int m_NodeIndex;
	};

	class ShaderGraphNode
	{
	public:
		ShaderGraphNode(unsigned int ID, glm::vec2 position = glm::vec2(0));

	public:
		unsigned int m_NodeID;

		virtual void Compile() = 0;

		std::vector<ShaderGraphConnection> m_Inputs;
		std::vector<ShaderGraphConnection> m_Outputs;

		// Editor QOL
		glm::vec2 m_Position;
	};



	class OutputNode : public ShaderGraphNode
	{
	public:
		OutputNode(unsigned int ID, glm::vec2 position = glm::vec2(0));

		void Compile() override {};
	};

	class TextureNode : public ShaderGraphNode
	{
	public:
		TextureNode(unsigned int ID, glm::vec2 position = glm::vec2(0));

		void Compile() override {};
	};

	class FloatNode : public ShaderGraphNode
	{
	public:
		FloatNode(unsigned int ID, glm::vec2 position = glm::vec2(0));

		void Compile() override {};
	};
}