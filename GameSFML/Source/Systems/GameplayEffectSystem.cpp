#pragma once
#include "../../System/GameplayEffectSystem.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../System/WorldTimerSystem.h"
void GameplayEffectSystem::Update(entt::registry& ECS)
{
	float dt = ECS.ctx<WorldTimer>().dt;

	DurationSystem(ECS, dt);
	IntervalTickSystem(ECS, dt);
	StackSystem(ECS, dt);
	ExecutionSystem(ECS);
	DeleteEffectSystem(ECS);
}

void GameplayEffectSystem::BeginPlay(entt::registry& ECS)
{
	ECS.set<GAS::ObserverTick>().connect(ECS, entt::collector.replace<GAS::TickTimes>().where<GAS::TickCapacity>());
}

void GameplayEffectSystem::DeleteEffectSystem(entt::registry& ECS) const
{
	/*ECS.view<GAS::DeleteEffect, GAS::RestoreStats, GAS::EffectInfo>().each([&ECS](auto entity, auto& stat, auto& info) {
		auto& Set = ECS.get<STATS::Set>(info.target);
		for (auto& s : stat)
		{
			Set[s.first] -= s.second;
		}
		});*/
	
	auto view = ECS.view<GAS::DeleteEffect>();
	ECS.destroy(view.begin(), view.end());
}

void GameplayEffectSystem::IntervalTickSystem(entt::registry& ECS, float dt) const
{
	auto view = ECS.view<GAS::IntervalTick>();
	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&ECS, dt, &view](auto entity) {
		auto& tick = view.get<GAS::IntervalTick>(entity);
		tick.curTime += dt;
		GAS::TickTimes nTicks = 0;
		while (tick.curTime >= tick.intervalTime)
		{
			tick.curTime -= tick.intervalTime;
			nTicks++;
		}
		if (nTicks > 0)
			ECS.replace<GAS::TickTimes>(entity, [&nTicks](auto& tick) { tick = nTicks; });
		});

	auto& obs = ECS.ctx<GAS::ObserverTick>();
	for (auto& entity : obs)
	{
		auto [nTick, tickCap] = ECS.get<GAS::TickTimes, GAS::TickCapacity>(entity);
		if (nTick + tickCap.curTick >= tickCap.maxTick)
		{
			nTick -= nTick + tickCap.curTick - tickCap.maxTick;
			tickCap.curTick = tickCap.maxTick;
			ECS.assign<GAS::DeleteEffect>(entity);
		}
		else
		{
			tickCap.curTick += nTick;
		}
		ECS.assign<GAS::ExecutionTimes>(entity, nTick);
	}
}

void GameplayEffectSystem::DurationSystem(entt::registry& ECS, float dt) const
{
	ECS.view<GAS::DurationLimitedTime>().each([&ECS, dt](auto entity, GAS::DurationLimitedTime& duration) {
		duration.curTime += dt;
		if (duration.curTime >= duration.maxTime)
			ECS.assign<GAS::DeleteEffect>(entity);
		});
	ECS.view<GAS::DurationInstant>().each([&ECS, dt](auto entity, auto) {
		ECS.assign<GAS::DeleteEffect>(entity);
		ECS.assign<GAS::ExecutionTimes>(entity, 1);
		});
}

void GameplayEffectSystem::StackSystem(entt::registry& ECS, float dt) const
{
	{
		auto view = ECS.view<GAS::AddStack, GAS::Stack>();
		std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&ECS, &view](auto entity) {
			auto [add, stack] = view.get<GAS::AddStack, GAS::Stack>(entity);
			stack.curStack += add;

			if (stack.curStack > stack.maxStack)
			{
				if (auto* sLost = ECS.try_get<GAS::StackLost>(entity); sLost)
				{
					sLost->curTime = sLost->maxTime;
				}
			}
			});
	}
	

	ECS.view<GAS::StackLost, GAS::Stack>().each([&ECS, dt](auto entity, GAS::StackLost& sLost, GAS::Stack& stack) {
		sLost.curTime -= dt;
		while (sLost.curTime <= 0.0f)
		{
			stack.curStack--;
			if (stack.curStack == 0)
			{
				ECS.assign<GAS::DeleteEffect>(entity);
				break;
			}
			sLost.curTime = sLost.maxTime;
		}
	});
}

void GameplayEffectSystem::ExecutionSystem(entt::registry& ECS) const
{
	ECS.view<GAS::ExecutionTimes>().each([&ECS](auto entity, GAS::ExecutionTimes& nExes) {
		

		
		});
}
