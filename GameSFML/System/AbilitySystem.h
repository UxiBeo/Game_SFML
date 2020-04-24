#pragma once
#include "../System/ISystemECS.h"
class AbilitySystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) const final;
	void BeginPlay(entt::registry& ECS) const final;
private:
	void Cooldown(entt::registry& ECS, float dt) const;
	void TryActiveAbility(entt::registry& ECS) const;
	void CommitAbility(entt::registry& ECS) const;
	void EndAbility(entt::registry& ECS) const;
};