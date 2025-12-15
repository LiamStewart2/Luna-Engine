#pragma once

#include <unordered_map>

#include "../ECS/ECS.h"

namespace Luna
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(EntityComponentSystem* ECS);
	private:

	};
}

