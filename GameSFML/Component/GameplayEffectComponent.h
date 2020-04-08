#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "AttributeComponent.h"
#include "GameplayTag.h"
namespace GAS
{
	struct EffectTags
	{
		Tag::Bitfiled tags;
		// Cancels any already-executing Effect with Tags.
		Tag::Bitfiled cancelWithTag;
		//While this Effect is executing, the owner of the Effect will be granted this set of Tags.
		Tag::Bitfiled act_OwnedTags;
		//The Effect can only be activated if the Target has all of these Tags.
		Tag::Bitfiled act_RequiredTags;
		//The Effect can only be activated if the activating Actor or Component does not have any of these Tags.
		Tag::Bitfiled act_BlockedTags;
		//The Ability can only be activated if the target Actor or Component does not have any of these Tags.
		Tag::Bitfiled target_RequiredTags;
		// Same as Target Required Tags but with Blocked tags.
		Tag::Bitfiled target_BlockedTags;
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
		unsigned int curTick = 0;
		unsigned int maxTick = 0;
	};
	using TickTimes = unsigned int;
	using ObserverTick = entt::observer;

	//Stack
	struct Stack
	{
		unsigned int curStack = 0;
		unsigned int maxStack = 0;
	};
	struct StackLost
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
	};
	using AddStack = unsigned int;
	struct BeginEffect{};
	struct EffectInfo
	{
		entt::hashed_string texturePath;
		entt::entity source = entt::null;
		entt::entity target = entt::null;
		std::vector<std::pair<RPGS::AttributeType, float>> captureAtts;
		bool firstTime = true;
		std::vector<std::pair<RPGS::AttributeType, RPGS::Value>> modifiedAtts;
	};

	template <RPGS::AttributeType T>
	struct RestoreAttribute
	{
		RPGS::Value value;
	};
	struct ExecutionTimes
	{
		unsigned int data;
	};
	struct DeleteEffect{};
}