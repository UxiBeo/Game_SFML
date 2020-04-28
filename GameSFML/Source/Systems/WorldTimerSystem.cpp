#pragma once
#include "../../System/WorldTimerSystem.h"
#include "../../Component/ParentChildrenComponent.h"
void WorldTimerSystem::BeginPlay(entt::registry& ECS)const
{
	ECS.set<Timer::World>();
}

void WorldTimerSystem::Update(entt::registry& ECS)const
{
	auto& worldTime = ECS.ctx<Timer::World>();
	worldTime.time = worldTime.clock.restart();
	worldTime.dt = std::min(float(worldTime.time.asMilliseconds()) / 1000.0f, worldTime.maxdt);
	worldTime.worldTimer += worldTime.dt;
	float dt = worldTime.dt;
	
	ECS.view<Timer::LifeTimeComponent>().each([&ECS, dt](auto entity, Timer::LifeTimeComponent& lt) {
		lt.curtime += dt;
		if (lt.curtime >= lt.maxTime)
		{
			ECS.destroy(entity);
		}
		});
	ECS.view<Timer::OneTime>().each([dt, &ECS](auto entity, auto& timer) {
		if (!timer.delegate)
			timer.curTime += dt;
		if (timer.curTime >= timer.maxTime)
		{
			if (timer.delegate)
				timer.delegate(entity, ECS);

			ECS.assign<Timer::SelfDelete>(entity);
		}

		});

	ECS.view<Timer::Loop>().each([dt, &ECS](auto entity, auto& timer) {
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
					ECS.assign<Timer::SelfDelete>(entity);
				}
			}
		}

		});

	auto view = ECS.view<Timer::SelfDelete>();
	ECS.destroy(view.begin(), view.end());
}
