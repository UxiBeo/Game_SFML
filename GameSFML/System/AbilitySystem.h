#pragma once
#include "../System/ISystemECS.h"
class AbilitySystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
private:
	void Cooldown(entt::registry& ECS, float dt) const;
};