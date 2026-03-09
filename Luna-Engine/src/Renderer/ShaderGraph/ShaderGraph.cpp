#include "ShaderGraph.h"

#include <string>

namespace Luna
{
	ShaderGraph::ShaderGraph()
	{
		m_OutputNode = new OutputNode(0, glm::vec2(0, 0));
	}

	ShaderGraph::~ShaderGraph()
	{
		DeleteNode(m_OutputNode->m_NodeID);
	}

	void ShaderGraph::SpawnNode(ShaderGraphNode* SpawnedNode)
	{
		m_Nodes.push_back(SpawnedNode);
	}


	void ShaderGraph::ConnectNodes(ShaderGraphNode* LeftSideNode, int OutputSlotIndex, ShaderGraphNode* RightSideNode, int InputSlotIndex)
	{
		LeftSideNode->m_Outputs[OutputSlotIndex] = ShaderGraphConnection(RightSideNode, InputSlotIndex);
		RightSideNode->m_Inputs[InputSlotIndex] = ShaderGraphConnection(LeftSideNode, OutputSlotIndex);
	}
	void ShaderGraph::BreakConnection(ShaderGraphNode* LeftSideNode, int OutputSlotIndex, ShaderGraphNode* RightSideNode, int InputSlotIndex)
	{
		LeftSideNode->m_Outputs[OutputSlotIndex].m_Connection = nullptr;
		RightSideNode->m_Inputs[InputSlotIndex].m_Connection = nullptr;
	}


	void ShaderGraph::DeleteNode(unsigned int NodeID)
	{
		ShaderGraphNode* node = FindNodeByID(NodeID);

		if(node == nullptr)
			return;

		// Clear Inputs
		for (int i = 0; i < node->m_Inputs.size(); i++)
			BreakConnection(node->m_Inputs[i].m_Connection, node->m_Inputs[i].m_NodeIndex, node, i);
		// Clear Outputs
		for(int i = 0; i < node->m_Outputs.size(); i++)
			BreakConnection(node, i, node->m_Outputs[i].m_Connection, node->m_Outputs[i].m_NodeIndex);

		delete node;
	}

	ShaderGraphNode* ShaderGraph::FindNodeByID(unsigned int NodeID)
	{
		for (int i = 0; i < m_Nodes.size(); i++)
		{
			if (m_Nodes[i]->m_NodeID == NodeID)
				return m_Nodes[i];
		}
		return nullptr;
	}
}