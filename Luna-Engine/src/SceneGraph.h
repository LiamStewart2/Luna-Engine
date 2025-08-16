#pragma once

#include <vector>

class SceneGraphNode
{
public:
	SceneGraphNode(unsigned int gameObject);
	
	// node - the node which will own the game object
	SceneGraphNode* GetNode(unsigned int gameObject, SceneGraphNode* parentNode);

	// ParentNode - the node which will own the game object
	void InsertNode(unsigned int gameObject, SceneGraphNode* parentNode = nullptr);
	// node - the node which will be removed, if nullptr, the current node will be removed
	void RemoveNode(unsigned int gameObject, SceneGraphNode* node = nullptr);

	std::vector<SceneGraphNode*>* getNodes() { return &m_Nodes; }
	unsigned int GetGameObject() {return m_GameObject;}
private:
	std::vector<SceneGraphNode*> m_Nodes = std::vector<SceneGraphNode*>();
	SceneGraphNode* m_ParentNode = nullptr;
	unsigned int m_GameObject = 0;
};

class SceneGraph : public SceneGraphNode
{
public:
	SceneGraph(unsigned int gameObject = 0);
	~SceneGraph();
};