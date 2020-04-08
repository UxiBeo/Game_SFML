#pragma once
#include "../../System/WorldTimerSystem.h"

void WorldTimerSystem::BeginPlay(entt::registry& ECS)
{
	ECS.on_destroy<CurrentActiveTimer>().connect<&WorldTimerSystem::DestroyCurrentActiveTimer>();
}

void WorldTimerSystem::Update(entt::registry& ECS)
{
	auto* worldTime = ECS.try_ctx<WorldTimer>();
	if (worldTime == nullptr) return;
	worldTime->dt = std::min(float(worldTime->clock.restart().asMilliseconds()) / 1000.0f, worldTime->maxdt);
	worldTime->worldTimer += worldTime->dt;
	float dt = worldTime->dt;

	ECS.view<OneTimeTimer>().each([dt, &ECS](auto entity, OneTimeTimer& timer) {
		if (!timer.delegate)
			timer.curTime += dt;
		if (timer.curTime >= timer.maxTime)
		{
			if (timer.delegate)
				timer.delegate(entity, ECS);

			ECS.assign<SelfDeleteTimer>(entity, timer.owner);
		}

		});

	ECS.view<LoopTimer>().each([dt, &ECS](auto entity, LoopTimer& timer) {
		if (!timer.delegate) return;

		timer.curTime += dt;
		while (timer.curTime >= timer.intervalTime)
		{
			timer.curTime -= timer.intervalTime;
			if (timer.delegate)
				timer.delegate(entity, ECS);
			if (!timer.isInfinity)
			{
				timer.curLoop++;
				if (timer.curLoop >= timer.intervalTime)
				{
					ECS.assign<SelfDeleteTimer>(entity, timer.owner);
				}
			}
		}

		});
	auto view = ECS.view<SelfDeleteTimer>();
	view.each([&ECS](auto entity, SelfDeleteTimer& timer) {

		auto* con = ECS.try_get<CurrentActiveTimer>(timer.owner);
		if (con)
		{
			con->RemoveEntity(entity);
		}
		});
	ECS.destroy(view.begin(), view.end());
}
