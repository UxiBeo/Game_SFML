#pragma once
#include "../AbilityComponent.h"
#include "../PhysicComponent.h"
#include "../ControllerComponent.h"
#include "../AnimationComponent.h"
#include "../GameplayEffectComponent.h"
#include "../DrawDebugComponent.h"
struct MeleeAttack
{
	static void OnAbilityStart(const GAS::AbilityComponent& ab, entt::registry& ECS)
	{
		ECS.assign<GAS::CommitAbility>(ab.self);
		ECS.assign<GAS::Activating>(ab.self);
		const auto& ma = ECS.get<MeleeAttack>(ab.self);
		AnimNotify an;
		an.triggerTime = ma.triggerTime;
		an.listenerE = ab.self;
		an.triggerD.connect<&MeleeAttack::OnAnimationNotify>();
		an.interrupD.connect<&MeleeAttack::OnAnimationInterrupt>();
		auto& ac = ECS.get<AnimationComponent>(ab.owner);
		auto iSet = ac.iSet;
		if (iSet <= 7)
		{
			iSet += 9;
		}
		else if (iSet == 8)
		{
			iSet = 9;
		}
		ac.frameTime = ac.ar->frameTime;
		ECS.assign<PlayAnimationMontage>(ab.owner, iSet, an);
		
	}
	static void OnAnimationInterrupt(const entt::entity& owner, const entt::entity& self, entt::registry& ECS)
	{
		ECS.remove<GAS::Activating>(self);
	}
	static void OnAnimationNotify(const entt::entity& owner, const entt::entity& self, entt::registry& ECS)
	{
		auto& pe = ECS.ctx<Physic::Engine>();
		const auto& gp = ECS.get<TargetPosition>(owner);
		const auto& ac = ECS.get<AnimationComponent>(owner);
		float nLength = 4.0f;
		float length = nLength / sqrt(2.0f);
		b2Vec2 boxExtent{ 2.0f,2.0f };
		b2Vec2 pos{ 0.0f,0.0f };
		constexpr uint8_t step = 9;
		switch (ac.iSet)
		{
		case 0 + step:
			pos.x = nLength;
			break;
		case 1 + step:
			pos = { length , -length };
			break;
		case 2 + step:
			pos.y = -nLength;
			break;
		case 3 + step:
			pos = { -length, -length };
			break;
		case 4 + step:
			pos.x = -nLength;
			break;
		case 5 + step:
			pos = { -length, length };
			break;
		case 6 + step:
			pos.y = nLength;
			break;
		case 7 + step:
			pos = { length, length };
			break;
		default:
			break;
		}
		
		b2AABB aabb;
		pos += gp.curPos;
		aabb.lowerBound = pos - boxExtent;
		aabb.upperBound = pos + boxExtent;
		auto& db = ECS.assign<DrawDebugComponent>(ECS.create());
		db.vertices.resize(4);
		db.vertices.emplace_back(aabb.lowerBound.x, aabb.upperBound.y);
		db.vertices.emplace_back(aabb.upperBound);
		db.vertices.emplace_back(aabb.upperBound.x, aabb.lowerBound.y);
		db.vertices.emplace_back(aabb.lowerBound);

		Physic::Query query;
		const auto& entities = query.GetEntities(pe, aabb);
		const auto* mes = ECS.try_get<GAS::ModifiedEffect>(self);
		if (mes && mes->eEffect.size() > 0)
		{
			for (auto entity : entities)
			{
				for (auto me : mes->eEffect)
				{
					auto& te = ECS.assign<GES::TryAppyEffect>(entity);
					te.prefapEntity = me;
					te.source = self;
					te.target = entity;
				}
			}
		}
		ECS.remove<GAS::Activating>(self);
	}
	float triggerTime = 0.0f;
};