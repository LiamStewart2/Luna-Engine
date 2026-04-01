#include "ShaderGraphNode.h"

namespace Luna
{
	ShaderGraphNode::ShaderGraphNode(unsigned int ID, glm::vec2 position)
	{
		m_NodeID = ID;
		m_Position = position;
	}

	OutputNode::OutputNode(unsigned int ID, glm::vec2 position) : ShaderGraphNode(ID, position)
	{

	}

	TextureNode::TextureNode(unsigned int ID, glm::vec2 position) : ShaderGraphNode(ID, position)
	{

	}

	FloatNode::FloatNode(unsigned int ID, glm::vec2 position) : ShaderGraphNode(ID, position)
	{

	}
}