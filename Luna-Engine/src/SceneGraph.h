#pragma once

#include <vector>

class SceneGraphNode
{
public:
	SceneGraphNode(unsigned int gameObject);
	
	// node - the node which will own the game object
	SceneGraphNode* GetNode(unsigned int gameObject, SceneGraphNode* node);

	// ParentNode - the node which will own the game object
	void InsertNode(unsigned int gameObject, SceneGraphNode* parentNode = nullptr);
	// parentNode - the node which owns the Scene Graph Node you wish to remove
	void RemoveNode(unsigned int gameObject, SceneGraphNode* parentNode = nullptr);
	void KillBranch(unsigned int gameObject, SceneGraphNode* parentNode = nullptr);

	std::vector<SceneGraphNode*>* getNodes() { return &m_Nodes; }

private:
	std::vector<SceneGraphNode*> m_Nodes;
	SceneGraphNode* m_ParentNode = nullptr;
	unsigned int m_GameObject = 0;
};

class SceneGraph : public SceneGraphNode
{
public:
	SceneGraph(unsigned int gameObject = 0);
	~SceneGraph();
};