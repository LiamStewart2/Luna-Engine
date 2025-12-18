#pragma once

#include <unordered_map>

#include "../Core/Timer.h"
#include "../ECS/ECS.h"

namespace Luna
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(EntityComponentSystem* ECS, bool runtime);
	private:
		Timer timer;
	};
}

