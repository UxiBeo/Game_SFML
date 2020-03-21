#pragma once
#include "../Component/PhysicComponent.h"
#include "../System/ISystemECS.h"

class PhysicSystem : public ISystemECS
{
	void Update(entt::registry& ECS, float dt) final
	{
		if (auto* physicEngine = ECS.try_ctx<PhysicEngine>(); physicEngine)
		{
			physicEngine->Engine->Step(dt, 4, 2);
		}
	}
};