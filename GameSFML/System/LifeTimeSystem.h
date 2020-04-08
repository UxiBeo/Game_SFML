#pragma once
#include "../System/ISystemECS.h"
#include "../System/WorldTimerSystem.h"
class LifeTimeSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS)
	{
		float dt = ECS.ctx<Timer::World>().dt;
		/*ECS.view<LifeTimeComponent>().each([&ECS,dt](auto entity, LifeTimeComponent& lifeTime) {
			lifeTime.curTime += dt;
			if (lifeTime.curTime >= lifeTime.maxLifeTime)
			{
				ECS.destroy(entity);
			}
		});*/
	}
};