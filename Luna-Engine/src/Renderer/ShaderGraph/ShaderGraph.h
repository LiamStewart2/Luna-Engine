#pragma once

#include "ShaderGraphNode.h"

namespace Luna
{
	class ShaderGraph
	{
	public:
		ShaderGraph();
		~ShaderGraph();

		// Head allocated Spawned Node
		void SpawnNode(ShaderGraphNode* SpawnedNode);
		// Left side node is the node we are connecting from
		// Right side node is the node we are connecting to
		void ConnectNodes(ShaderGraphNode* LeftSideNode, int OutputSlotIndex, ShaderGraphNode* RightSideNode, int InputSlotIndex);
		// Deletes the connection between two nodes
		void BreakConnection(ShaderGraphNode* LeftSideNode, int OutputSlotIndex, ShaderGraphNode* RightSideNode, int InputSlotIndex);

		// Deletes the node, removing all connection in the process
		void DeleteNode(unsigned int NodeID);

		void Compile();

	private:
		OutputNode* m_OutputNode;
		std::vector<ShaderGraphNode*> m_Nodes;

		// Returns nullptr if no nodes with NodeID == ID
		ShaderGraphNode* FindNodeByID(unsigned int ID);

	};
}

