#pragma once

#include <unordered_map>

#include "GLM/gtx/quaternion.hpp"
#include "GLM/vec3.hpp"
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

