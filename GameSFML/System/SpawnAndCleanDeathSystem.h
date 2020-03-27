#pragma once
#include "../System/ISystemECS.h"
#include "../System/WorldTimerSystem.h"
#include "../Component/SpawnComponent.h"
#include "../Locator.h"
class SpawnSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) final
	{
		auto* worldTime = ECS.try_ctx<WorldTimer>();
		if (worldTime == nullptr) return;
		float dt = worldTime->dt;
		/*ECS.view<SpawnComponent, UpdateSpawnComponent>().each([&ECS, dt](auto entity, SpawnComponent& spawn, UpdateSpawnComponent& update) {
			if (!spawn.bIsEnable) return;

			spawn.curTime += dt;
			while (spawn.curTime >= spawn.interval)
			{
				spawn.curTime -= spawn.interval;
				update.myDelegate(entity, ECS);
			}
		});*/
	}
};