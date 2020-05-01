#pragma once
#include "../../System/AbilitySystem.h"
#include"../../Component/AbilityComponent.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../Component/TimerComponent.h"
void AbilitySystem::Update(entt::registry& ECS) const
{
	const float dt = ECS.ctx<Timer::World>().dt;
	Cooldown(ECS, dt);
	TryActiveAbility(ECS);
	CommitAbility(ECS);
}

void AbilitySystem::BeginPlay(entt::registry& ECS) const
{
	
}

void AbilitySystem::Cooldown(entt::registry& ECS, float dt) const
{
	ECS.view<GAS::DoingCooldown, GAS::CooldownComponent>().each([&ECS,dt](auto entity, auto, auto& cd) {
		cd.curTime += dt;
		if (cd.curTime >= cd.maxTime)
		{
			cd.curTime = 0.0f;
			ECS.remove<GAS::DoingCooldown>(entity);
		}
		});
}

void AbilitySystem::TryActiveAbility(entt::registry& ECS) const
{
	ECS.view<GAS::TryActivateAbility, GAS::AbilityComponent>().each([&ECS](auto entity, auto, GAS::AbilityComponent& ac) {
		if (ECS.has<GAS::Activating>(entity) || ECS.has<GAS::DoingCooldown>(entity)) return;
		if ((ac.tag.self_BlockTags & ac.tag.self_tag) > 0 ||
			(ac.tag.self_RequiredTags & ac.tag.self_tag) != ac.tag.self_RequiredTags) return;
		const auto& tags = ECS.get<TagComponent>(ac.owner).tags;
		if ((ac.tag.owner_BlockTags & tags) > 0 ||
			(ac.tag.owner_RequiredTags & tags) != ac.tag.owner_RequiredTags) return;
		assert(ac.mrD);
		if (ac.mrD) ac.mrD(ac, ECS);
		});
	ECS.clear<GAS::TryActivateAbility>();
}

void AbilitySystem::CommitAbility(entt::registry& ECS) const
{
	auto view = ECS.view<GAS::CommitAbility, GAS::CooldownComponent>();
	for (auto e : view)
	{
		ECS.assign<GAS::DoingCooldown>(e);
	}

	ECS.clear<GAS::CommitAbility>();
}
