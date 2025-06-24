#pragma once

#include <string>
#include <memory>

#include "GameObject.h"

class Component
{
public:
	Component(GameObject* _gameObject);

	virtual void OnStart() {}
	virtual void Update() {}
	virtual void OnDestroy() {}

protected:
	GameObject* gameObject = nullptr;

private:

};

