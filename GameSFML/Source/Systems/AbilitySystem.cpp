#pragma once
#include "../../System/AbilitySystem.h"
#include"../../Component/AbilityComponent.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../Component/TimerComponent.h"
void AbilitySystem::Update(entt::registry& ECS) const
{
	const float dt = ECS.ctx<Timer::World>().dt;
	TryActiveAbility(ECS);
	CommitAbility(ECS);
	Cooldown(ECS, dt);
	EndAbility(ECS);
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
	ECS.view<GAS::TryActivateAbility, GAS::AbilityComponent>(entt::exclude<GAS::DoingCooldown>).each([&ECS](auto entity, auto, const GAS::AbilityComponent& ac) {

		auto& sourceTag = ECS.get<Tag::Bitfiled>(ac.source);
		if ((sourceTag & ac.tagSet.source_BlockTags) > 0)
			return;
		if ((sourceTag & ac.tagSet.source_RequiredTags) != ac.tagSet.source_RequiredTags)
			return;
		if (const GAS::CostComponent* cost = ECS.try_get<GAS::CostComponent>(entity); cost)
		{
			const auto& ap = ECS.get<GES::AttributePack>(ac.source);
			if ((ap.bitmask & cost->attributeName) > 0)
			{
				switch (cost->attributeName)
				{
				case GES::AttributeType::HealthPoint:
				{
					auto& sv = ECS.get<GES::SpecialValue>(ap.attribute[(uint32_t)GES::AttributeType::HealthPoint]);
					sv.reserve += cost->amount;
					if (sv.curValue < sv.reserve)
						return;
				}
					break;
				case GES::AttributeType::ManaPoint:
				{
					auto& sv = ECS.get<GES::SpecialValue>(ap.attribute[(uint32_t)GES::AttributeType::ManaPoint]);
					sv.reserve += cost->amount;
					if (sv.curValue < sv.reserve)
						return;
				}
					break;
				}
			}
		}

		sourceTag |= ac.tagSet.onStart_Source_GrandTags;
		sourceTag &= ~ac.tagSet.onStart_Source_RemoveTags;

		auto& le = ECS.get<GAS::Listener<GAS::Event::OnAbilityStart>>(entity);
		if (le.mrD)
			le.mrD(GAS::EventInfo<GAS::Event::OnAbilityStart>{ ECS, entity, entity });
		});
	ECS.clear<GAS::TryActivateAbility>();
}

void AbilitySystem::CommitAbility(entt::registry& ECS) const
{
	auto view = ECS.view<GAS::CommitAbility, GAS::AbilityComponent, GAS::CooldownComponent>();
	for (auto e : view)
	{
		ECS.assign<GAS::DoingCooldown>(e);
	}
	ECS.view<GAS::CommitAbility, GAS::AbilityComponent, GAS::CostComponent>().each([&ECS](auto entity,
		auto, const GAS::AbilityComponent& ac, const GAS::CostComponent& cc) {
			auto& ap = ECS.get<GES::AttributePack>(ac.source);
			switch (cc.attributeName)
			{
			case GES::AttributeType::HealthPoint:
			{
				auto& sv = ECS.get<GES::SpecialValue>(ap.attribute[(uint32_t)GES::AttributeType::HealthPoint]);
				sv.reserve = 0.0f;
				sv.curValue -= cc.amount;
			}
			break;
			case GES::AttributeType::ManaPoint:
			{
				auto& sv = ECS.get<GES::SpecialValue>(ap.attribute[(uint32_t)GES::AttributeType::ManaPoint]);
				sv.reserve = 0.0f;
				sv.curValue -= cc.amount;
			}
			break;
			}
		});

	ECS.clear<GAS::CommitAbility>();
}

void AbilitySystem::EndAbility(entt::registry& ECS) const
{
	ECS.view<GAS::EndAbility, GAS::AbilityComponent>().each([&ECS](auto entity, auto, const GAS::AbilityComponent& ac) {

		auto& sourceTag = ECS.get<Tag::Bitfiled>(ac.source);

		sourceTag |= ac.tagSet.onEnd_Source_GrandTags;
		sourceTag &= ~ac.tagSet.onEnd_Source_RemoveTags;
		});
	ECS.clear<GAS::EndAbility>();
}
