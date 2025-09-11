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
	}
	return nullptr;
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

bool SceneGraphNode::RemoveNode(unsigned int gameObject, EntityComponentSystem* ECS, std::vector<unsigned int>* gameObjects)
{
	if (m_GameObject == gameObject)
	{
		if (m_ParentNode)
		{
			auto& siblings = m_ParentNode->m_Nodes;
			for (size_t i = 0; i < siblings.size(); i++)
			{
				if (siblings[i] == this)
				{
					this->DestroySubtree(ECS, gameObjects);
					delete this;
					siblings.erase(siblings.begin() + i);
					return true;
				}
			}
			return false;
		}
		else
		{
			this->DestroySubtree(ECS, gameObjects);
			return true;
		}
	}

	for (size_t i = 0; i < m_Nodes.size(); i++)
	{
		if(m_Nodes[i]->RemoveNode(gameObject, ECS, gameObjects))
			return true;
	}
	return false;
}

void SceneGraphNode::DestroySubtree(EntityComponentSystem* ECS, std::vector<unsigned int>* gameObjects)
{
	// Destroy all children first
	for (SceneGraphNode* child : m_Nodes)
	{
		child->DestroySubtree(ECS, gameObjects);
		delete child;
	}
	m_Nodes.clear();

	// Remove ECS components and gameObject listing
	if (ECS) ECS->RemoveAllComponents(m_GameObject);

	if (gameObjects)
	{
		auto it = std::find(gameObjects->begin(), gameObjects->end(), m_GameObject);
		if (it != gameObjects->end()) gameObjects->erase(it);
	}
}


SceneGraph::SceneGraph(unsigned int gameObject) : SceneGraphNode(gameObject)
{
}

SceneGraph::~SceneGraph()
{
	RemoveNode(0, nullptr); // Remove all nodes starting from the root
}
