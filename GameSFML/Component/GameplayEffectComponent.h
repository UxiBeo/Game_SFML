#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "AttributeComponent.h"

namespace GAS
{
	struct GameplayEffectComponent
	{
		entt::entity applyOnTarget;
	};
	struct OutOfStack {};
	struct OutOfTick {};
	struct TickAble
	{
		float curTime = 0.0f;
		float intervalTime = 0.0f;
		unsigned int maxTick = 1;
		unsigned int curTick = 0;
	};
	using NumberOfTick = unsigned int;
	using AddStack = unsigned int;
	struct StackAble
	{
		unsigned int curStack = 0;
		unsigned int maxStack = 0;
		float curTime = 0.0f;
		float stackTime = 0.0f;
		bool isCleanWhenOutOfTime = false;
	};
}