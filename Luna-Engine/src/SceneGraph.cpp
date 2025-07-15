#include "SceneGraph.h"

SceneGraphNode::SceneGraphNode(unsigned int gameObject = 0)
{
	m_GameObject = gameObject;
}

SceneGraphNode::~SceneGraphNode()
{
}

SceneGraphNode* SceneGraphNode::GetNode(unsigned int gameObject, SceneGraphNode* node = nullptr)
{
	if(m_GameObject = gameObject)
		return this;

	if (node == nullptr || node == this)
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
		m_Nodes.push_back(new SceneGraphNode(gameObject));
	else
		parentNode->InsertNode(gameObject, nullptr);
}

void SceneGraphNode::RemoveNode(unsigned int gameObject, SceneGraphNode* parentNode)
{
	if (parentNode == nullptr || parentNode == this)
	{
		int index = -1;	SceneGraphNode* node = nullptr;
		for (int i = 0; i < m_Nodes.size(); i++)
		{
			if(m_Nodes[i]->m_GameObject == gameObject)
			{
				index = i; 
				node = m_Nodes[i];
			}
		}

		if(index != -1 && node != nullptr)
		{
			m_Nodes.erase(m_Nodes.begin() + index);
			delete node;
		}
	}
	else
	{
		parentNode->RemoveNode(gameObject, parentNode);
	}
}


SceneGraph::SceneGraph(unsigned int gameObject = 0) : SceneGraphNode(gameObject)
{
}