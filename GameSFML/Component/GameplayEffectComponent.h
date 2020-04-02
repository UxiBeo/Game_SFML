#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "AttributeComponent.h"

namespace GAS
{
	struct DurationInstant{};
	struct DurationInfnity{};

	struct DurationLimitedTime
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
	};

	struct IntervalTick
	{
		float curTime = 0.0f;
		float intervalTime = 0.0f;
		unsigned int curTick = 0;
		unsigned int maxTick = 0;
	};
	using NumberTick = unsigned int;
	struct DeleteEffect{};
	/*struct ApplyGameplayEffect
	{
		entt::hashed_string effectName;
		entt::delegate<void(entt::entity, ApplyGameplayEffect&, entt::registry&)> applyDelegate;
		entt::entity owner;
		entt::entity target;
	};
	struct TickAble
	{
		float curTime = 0.0f;
		float intervalTime = 0.0f;
		unsigned int maxTick = 1;
		unsigned int curTick = 0;
		entt::delegate<void(entt::entity, TickAble&, entt::registry&, float dt)> tickDelegate;
	};
	struct StackAble
	{
		unsigned int curStack = 0;
		unsigned int maxStack = 0;
		float curTime = 0.0f;
		float stackTime = 0.0f;
		entt::delegate<void(entt::entity, StackAble&, entt::registry&, float dt)> stackDelegate;
	};*/
}