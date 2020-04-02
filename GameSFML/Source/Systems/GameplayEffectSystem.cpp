#pragma once
#include "../../System/GameplayEffectSystem.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../System/WorldTimerSystem.h"
void GameplayEffectSystem::Update(entt::registry& ECS)
{
	auto* worldTime = ECS.try_ctx<WorldTimer>();
	if (worldTime == nullptr) return;
	float dt = worldTime->dt;

	ECS.view<GAS::DurationLimitedTime>().each([&ECS, dt](auto entity, GAS::DurationLimitedTime& duration) {
		duration.curTime += dt;
		if (duration.curTime >= duration.maxTime)
			ECS.destroy(entity);
		});

	ECS.view<GAS::IntervalTick>().each([&ECS, dt](auto entity, GAS::IntervalTick& tick) {
		tick.curTime += dt;
		GAS::NumberTick nTicks = 0;
		entt::observer observer{ ECS, entt::collector.replace<GAS::NumberTick>()};
		while (tick.curTime >= tick.intervalTime)
		{
			tick.curTime -= tick.intervalTime;
			tick.curTick++;
			nTicks++;
			if (tick.curTick >= tick.maxTick)
			{
				ECS.assign<GAS::DeleteEffect>(entity);
				ECS.replace<GAS::NumberTick>(entity, [nTicks](GAS::NumberTick& tick) { tick = nTicks; });
				break;
			}
		}
		if (nTicks > 0)
			ECS.replace<GAS::NumberTick>(entity, [nTicks](GAS::NumberTick& tick) { tick = nTicks; });

		});



	{
		auto view = ECS.view<GAS::DeleteEffect>();
		ECS.destroy(view.begin(), view.end());
	}
	
}
