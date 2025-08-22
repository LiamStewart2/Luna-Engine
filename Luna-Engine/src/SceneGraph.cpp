#include "SceneGraph.h"

SceneGraphNode::SceneGraphNode(unsigned int gameObject = 0)
{
	m_GameObject = gameObject;
}

SceneGraphNode* SceneGraphNode::GetNode(unsigned int gameObject, SceneGraphNode* parentNode = nullptr)
{
	if(m_GameObject == gameObject)
		return this;

	if (parentNode == nullptr || parentNode == this)
	{
		for(SceneGraphNode* node : m_Nodes)
		{
			SceneGraphNode* n = node->GetNode(gameObject, node);
			if(n != nullptr)
				return n;
		}
		return nullptr;
	}
}

void SceneGraphNode::InsertNode(unsigned int gameObject, SceneGraphNode* parentNode)
{
	if (parentNode == nullptr || parentNode == this)
	{
		m_Nodes.push_back(new SceneGraphNode(gameObject));
		m_Nodes[m_Nodes.size() - 1]->m_ParentNode = this;
	}
	else
		parentNode->InsertNode(gameObject, nullptr);
}

void SceneGraphNode::RemoveNode(unsigned int gameObject, SceneGraphNode* node)
{
	if (node == nullptr || (node == this && gameObject == m_GameObject))
	{
		for(SceneGraphNode* childNode : m_Nodes)
		{
			childNode->RemoveNode(childNode->GetGameObject(), childNode);
			delete childNode;
		}
		m_Nodes.clear();
	}
}


SceneGraph::SceneGraph(unsigned int gameObject) : SceneGraphNode(gameObject)
{
}

SceneGraph::~SceneGraph()
{
	RemoveNode(0, nullptr); // Remove all nodes starting from the root
}
