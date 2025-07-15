#pragma once

#include <vector>

class SceneGraphNode
{

};

class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	// ParentNode - the node which will own the game object
	void InsertNode(SceneGraphNode* parentNode, unsigned int gameObject);
	// parentNode - the node which owns the game object you wish to remove
	void RemoveNode(SceneGraphNode* parentNode, unsigned int gameObject);

	std::vector<SceneGraphNode*>* getNodes() { return &nodes; }
private:
	std::vector<SceneGraphNode*> nodes;
};