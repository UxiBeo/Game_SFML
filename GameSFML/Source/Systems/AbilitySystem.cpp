#pragma once
#include "../../System/AbilitySystem.h"
#include"../../Component/AbilityComponent.h"
#include "../../Component/GameplayEffectComponent.h"

void AbilitySystem::Update(entt::registry& ECS)
{
	ECS.view<GAS::TryActivateAbility, GAS::TryActivateDelegate>().each([&ECS](auto entity, GAS::TryActivateDelegate& tryActive) {
		/*if (tryActive && tryActive(entity, ECS))
			if (auto* bahavior = ECS.try_get<GAS::ActivateBehaviorDelegate>(entity); bahavior && (*bahavior))
				(*bahavior)(entity, ECS);
		*/
		});
	ECS.clear<GAS::TryActivateAbility>();
	/*if (auto* obs = ECS.try_ctx<GAS::WaitTargetDataObserver>(); obs)
	{
		obs->each([&ECS](auto entity, GAS::WaitTargetData& target) {


			auto effectEntity = ECS.create();
			GAS::ApplyGameplayEffect GE;
			GE.owner = entity;
			GE.target = target;
			ECS.replace<GAS::ApplyGameplayEffect>(effectEntity, GE);
		});
	}*/

	/*if (auto* obs = ECS.try_ctx<GAS::WaitLocationObserver>(); obs)
	{
		obs->each([&ECS](auto entity, GAS::WaitLocation& location) {
			
			});
	}*/

}

void AbilitySystem::BeginPlay(entt::registry& ECS)
{
	ECS.set<GAS::WaitTargetDataObserver>().connect(ECS, entt::collector.replace<GAS::WaitTargetData>());
	ECS.set<GAS::WaitLocationObserver>().connect(ECS, entt::collector.replace<GAS::WaitLocation>());
}

void AbilitySystem::Cooldown(entt::registry& ECS, float dt) const
{
	ECS.view<GAS::StartCooldown, GAS::CooldownComponent>().each([&ECS,dt](auto entity, auto, auto& cd) {
		cd.curTime += dt;
		if (cd.curTime >= cd.maxTime)
		{
			ECS.remove<GAS::StartCooldown>(entity);
		}
		});
}
