#pragma once

#include <vector>

#include "ECS/ECS.h"

class SceneGraphNode
{
public:
	SceneGraphNode(unsigned int gameObject);
	
	// node - the node which will own the game object
	SceneGraphNode* GetNode(unsigned int gameObject, SceneGraphNode* parentNode);

	// ParentNode - the node which will own the game object
	void InsertNode(unsigned int gameObject, SceneGraphNode* parentNode = nullptr);
	// node - the node which will be removed
	bool RemoveNode(unsigned int gameObject, EntityComponentSystem* ECS = nullptr, std::vector<unsigned int>* gameObjects = nullptr);

	std::vector<SceneGraphNode*>* getNodes() { return &m_Nodes; }
	unsigned int GetGameObject() {return m_GameObject;}
private:
	std::vector<SceneGraphNode*> m_Nodes = std::vector<SceneGraphNode*>();
	SceneGraphNode* m_ParentNode = nullptr;
	unsigned int m_GameObject = 0;

	void DestroySubtree(EntityComponentSystem* ECS, std::vector<unsigned int>* gameObjects);
};

class SceneGraph : public SceneGraphNode
{
public:
	SceneGraph(unsigned int gameObject = 0);
	~SceneGraph();
};