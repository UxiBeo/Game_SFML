#pragma once
#include "../Component/GameplayEffectComponent.h"
#include "../System/ISystemECS.h"
#include "WorldTimerSystem.h"
class GameplayEffectSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final
	{
		auto* worldTime = ECS.try_ctx<WorldTimer>();
		if (worldTime == nullptr) return;
		float dt = worldTime->dt;

		{
			auto view = ECS.view<GAS::AddStack, GAS::StackAble>();

			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
				auto [numAdd, stack] = view.get<GAS::AddStack, GAS::StackAble>(entity);
				stack.curStack += numAdd;
				stack.curTime = stack.stackTime;
				if (stack.curStack > stack.maxStack)
					stack.curStack = stack.maxStack;

				});
			ECS.clear<GAS::AddStack>();
		}
		
		
		ECS.view<GAS::StackAble>().each([dt,&ECS](auto entity, GAS::StackAble& stack) {
			stack.curTime -= dt;
			unsigned int curStack = stack.curStack;
			while (stack.curTime <= 0.0f)
			{
				if (stack.isCleanWhenOutOfTime)
				{
					ECS.assign<GAS::OutOfStack>(entity);
					break;
				}
				else
				{
					stack.curTime += stack.stackTime;
					stack.curStack--;
					if (stack.curStack == 0)
						ECS.assign<GAS::OutOfStack>(entity);
				}
			}
		});

		{
			auto view = ECS.view<GAS::AddStack, GAS::StackAble>();

			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
				auto [numAdd, stack] = view.get<GAS::AddStack, GAS::StackAble>(entity);
				stack.curStack += numAdd;
				stack.curTime = stack.stackTime;
				if (stack.curStack > stack.maxStack)
					stack.curStack = stack.maxStack;

			});
		}
		ECS.view<GAS::TickAble>().each([dt, &ECS](auto entity, GAS::TickAble& tick) {
			GAS::NumberOfTick nTick = 0;
			tick.curTime += dt;
			while (tick.curTime >= tick.intervalTime)
			{
				tick.curTime -= tick.intervalTime;
				tick.curTick++;
				nTick++;
				if (tick.curTick >= tick.maxTick)
				{
					ECS.assign<GAS::OutOfTick>(entity);
					break;
				}
			}
			if (nTick > 0)
				ECS.assign<GAS::NumberOfTick>(entity, nTick);
		});


	}
};