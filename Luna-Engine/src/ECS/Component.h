#pragma once

#include <string>
#include <memory>

class Renderer;
class GameObject;

class Component
{
public:
	Component(GameObject* _gameObject) : gameObject(_gameObject) {}

	virtual void OnStart() {}

	virtual void Update() {}
	virtual void OnRender(Renderer* renderer) {}

	virtual void OnDestroy() {}

protected:
	GameObject* gameObject = nullptr;

private:

};

