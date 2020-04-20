#pragma once
#include "../../System/AbilitySystem.h"
#include"../../Component/AbilityComponent.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../Component/TimerComponent.h"
void AbilitySystem::Update(entt::registry& ECS) const
{
	const float dt = ECS.ctx<Timer::World>().dt;
	TryActiveAbility(ECS);
	Cooldown(ECS, dt);
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
			ECS.remove<GAS::DoingCooldown>(entity);
		}
		});
}

void AbilitySystem::TryActiveAbility(entt::registry& ECS) const
{
	ECS.view<GAS::TryActivateAbility, GAS::AbilityComponent>().each([&ECS](auto entity, auto, const GAS::AbilityComponent& ac) {

		if (ECS.has<GAS::DoingCooldown>(entity))
			return;
		const auto& sourceTag = ECS.get<Tag::Bitfiled>(ac.source);
		if ((sourceTag & ac.tagSet.source_BlockTags) > 0)
			return;
		if ((sourceTag & ac.tagSet.source_RequiredTags) == ac.tagSet.source_RequiredTags)
			return;
		if (const GAS::CostComponent* cost = ECS.try_get<GAS::CostComponent>(entity); cost)
		{
			const auto& ap = ECS.get<GES::AttributePack>(cost->source);
			if ((ap.bitmask & cost->attributeName) > 0)
			{
				switch (cost->attributeName)
				{
				case GES::AttributeType::HealthPoint:
					if (ECS.get<GES::SpecialValue>(ap.attribute[(uint32_t)GES::AttributeType::HealthPoint]).curValue < cost->amount)
						return;
					break;
				case GES::AttributeType::ManaPoint:
					if (ECS.get<GES::SpecialValue>(ap.attribute[(uint32_t)GES::AttributeType::ManaPoint]).curValue < cost->amount)
						return;
					break;
				}
			}
		}
		
		if (auto* listener = ECS.try_get<GAS::Listener<GAS::Event::OnAbilityStart>>(entity); listener)
		{
			if (listener->detegate)
				listener->detegate(GAS::Trigger<GAS::Event::OnAbilityStart>{ entity, entity, ECS });
		}
		});
}
