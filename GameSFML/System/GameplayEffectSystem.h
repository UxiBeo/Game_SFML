#pragma once
#include "../System/ISystemECS.h"

class GameplayEffectSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
private:
	void DeleteEffectSystem(entt::registry& ECS) const;
	void IntervalTickSystem(entt::registry& ECS, float dt) const;
	void DurationSystem(entt::registry& ECS, float dt) const;
	void StackSystem(entt::registry& ECS, float dt) const;
	void ExecutionSystem(entt::registry& ECS) const;
};