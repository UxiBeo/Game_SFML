#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "AttributeComponent.h"
#include "GameplayTag.h"
namespace GES
{
	struct EffectTags
	{
		Tag::Bitfiled tags;

		Tag::Bitfiled target_RequiredTags;
		Tag::Bitfiled target_BlockTags;

		Tag::Bitfiled target_GrantTags;
		Tag::Bitfiled target_RemoveTags;
	};
	
	//Duration
	struct DurationInstant{};
	struct DurationInfnity{};
	struct DurationLimitedTime
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
	};

	//Interval Tick
	struct IntervalTick
	{
		float curTime = 0.0f;
		float intervalTime = 0.0f;
		
	};
	struct TickCapacity
	{
		uint8_t curTick = 0;
		uint8_t maxTick = 0;
	};
	struct Ticks
	{
		uint8_t value;
	};
	using ObserverTick = entt::observer;

	//Stack
	struct Stack
	{
		uint8_t minStack = 0;
		uint8_t curStack = 0;
		uint8_t maxStack = 0;
	};
	struct StackLost
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
	};
	struct AddStack
	{
		uint8_t value;
	};
	struct BeginEffect{};
	struct EffectInfo
	{
		EffectTags tags;
		struct ModifiedValue
		{
			RPGS::AttributeType type;
			RPGS::Value modValue;
			RPGS::Value storeValue;
		};
		struct CaptureValue
		{
			RPGS::AttributeType type;
			float value = 0.0f;
		};
		struct CostAndHealValue
		{
			RPGS::AttributeType type;
			float value = 0.0f;
		};
		entt::hashed_string effectName;
		entt::hashed_string texturePath;
		entt::entity source = entt::null;
		entt::entity target = entt::null;
		std::vector<CaptureValue> capture;
		std::vector<ModifiedValue> modified;
		std::vector<CostAndHealValue> costAndHealth;
	};
	
	struct Executions
	{
		uint8_t value;
	};
	struct TryAppyEffect{};
	struct SelfDelete{};
}