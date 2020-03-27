#pragma once
#include "../System/ISystemECS.h"
#include "../System/WorldTimerSystem.h"
class LifeTimeSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS)
	{
		auto* worldTime = ECS.try_ctx<WorldTimer>();
		if (worldTime == nullptr) return;
		float dt = worldTime->dt;
		/*ECS.view<LifeTimeComponent>().each([&ECS,dt](auto entity, LifeTimeComponent& lifeTime) {
			lifeTime.curTime += dt;
			if (lifeTime.curTime >= lifeTime.maxLifeTime)
			{
				ECS.destroy(entity);
			}
		});*/
	}
};