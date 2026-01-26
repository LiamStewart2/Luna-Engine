#pragma once

#include <string>
#include <GLM/glm.hpp>

#include "Component.h"

struct PhysicsComponent : public Component
{
	PhysicsComponent(unsigned int _gameObject = 0, bool simulate = true, float mass = 1.0f, float gravityValue = 9.81f, bool dynamic = true) :
		Component(_gameObject), m_Simulate(simulate), m_Dynamic(dynamic), m_Mass(mass), m_GravityValue(gravityValue)
	{
		
	}

	bool m_Simulate = true;
	bool m_Dynamic = true;
	bool m_BeingManipulated = false;

	float m_Mass = 1.0f;
	float m_GravityValue = 9.81f;
	
	glm::vec3 m_NetForce = glm::vec3(0, 0, 0);
	glm::vec3 m_Acceleration = glm::vec3(0, 0, 0);
	glm::vec3 m_Velocity = glm::vec3(0, 0, 0);

	glm::vec3 m_AngularVelocity = glm::vec3(0, 0, 0);
	glm::vec3 m_NetTorque = glm::vec3(0, 0, 0);
	glm::vec3 m_AngularAcceleration = glm::vec3(0, 0, 0);

	glm::mat3 m_InertiaTensor = glm::mat3(1);
};

enum ColliderShape
{
	Sphere,
	Cube
};

struct ColliderComponent : public Component
{
	ColliderComponent(unsigned int _gameObject = 0, ColliderShape shape = ColliderShape::Sphere, glm::vec3 colliderSize = glm::vec3(1)) :
		Component(_gameObject), m_Shape(shape), m_ColliderSize(colliderSize)
	{

	}

	ColliderShape m_Shape = ColliderShape::Sphere;

	glm::vec3 m_ColliderSize = glm::vec3(1);
	glm::vec3 m_HandledVelocity = glm::vec3(0);
};