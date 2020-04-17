#pragma once
#include "entt/entt.hpp"
#include "Component/GameplayTag.h"
struct EffectTags
{
	Tag::Bitfiled tags;

	Tag::Bitfiled target_RequiredTags;
	Tag::Bitfiled target_BlockTags;

	Tag::Bitfiled begin_target_GrantTags;
	Tag::Bitfiled begin_target_RemoveTags;

	Tag::Bitfiled end_target_GrantTags;
	Tag::Bitfiled end_target_RemoveTags;
};
class IStack
{
public:
	void Update(float dt) {};
	uint8_t GetCurrentStack() const
	{
		return curStack;
	}
private:
	uint8_t curStack = 1;
	uint8_t maxStack = 1;
};
class Effect
{
public:
	void Update(entt::entity entity, entt::registry& ECS)
	{
		float dt;
		stack->Update(dt);
	}
private:
	entt::hashed_string effectName;
	bool isDelete = false;
	class IDuration* duraion = nullptr;
	class IExecution* execution = nullptr;
	class IStack* stack = nullptr;
	EffectTags tag;
};