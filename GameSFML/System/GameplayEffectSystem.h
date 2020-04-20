#pragma once
#include "../System/ISystemECS.h"
#include "../Component/GameplayEffectComponent.h"
class GameplayEffectSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) const final;
	void BeginPlay(entt::registry& ECS) const final;
public:
	static void OnDestroyedCurrentEffect(entt::registry& ECS, entt::entity entity);
private:
	void DeleteEffect(entt::registry& ECS) const;
	void IntervalTickSystem(entt::registry& ECS, float dt) const;
	void DurationSystem(entt::registry& ECS, float dt) const;
	void StackSystem(entt::registry& ECS, float dt) const;
	void ExecutionSystem(entt::registry& ECS) const;
	void TryApplyEffect(entt::registry& ECS) const;
	void ApplyDame(entt::registry& ECS) const;
	void ModifiedAttribute(entt::registry& ECS) const;
	void ModifiedCurrentSpecial(entt::registry& ECS) const;
	void RestoreAttribute(entt::registry& ECS) const;
	void RenewEffectSystem(entt::registry& ECS) const;
};