#pragma once

struct Component
{
	Component() : gameObject(0) {}
	Component(unsigned int _gameObject) : gameObject(_gameObject) {}
	unsigned int gameObject;
};