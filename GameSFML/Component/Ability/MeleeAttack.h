#pragma once
#include "../AbilityComponent.h"
#include "../PhysicComponent.h"
#include "../ControllerComponent.h"
#include "../AnimationComponent.h"
#include "../GameplayEffectComponent.h"
#include "../../DrawDebugComponent.h"
struct MeleeAttack
{
	static void OnAbilityStart(const GAS::EventInfo<GAS::Event::OnAbilityStart>& e)
	{
		e.ECS.assign<GAS::CommitAbility>(e.listenerE);
		auto& pe = e.ECS.ctx<Physic::Engine>();
		const auto& ab = e.ECS.get<GAS::AbilityComponent>(e.listenerE);
		const auto& gp = e.ECS.get<TargetPosition>(ab.source);
		const auto& ac = e.ECS.get<AnimationComponent>(ab.source);
		float nLength = 2.0f;
		float length = nLength / sqrt(2.0f);
		b2Vec2 boxExtent{ 1.0f,1.0f };
		b2Vec2 pos{ 0.0f,0.0f };
		switch (ac.iSet)
		{
		case 0:
			pos.x = nLength;
			break;
		case 1:
			pos = { length , -length };
			break;
		case 2:
			pos.y = -nLength;
			break;
		case 3:
			pos = { -length, -length };
			break;
		case 4:
			pos.x = -nLength;
			break;
		case 5:
			pos = { -length, length };
			break;
		case 6:
			pos.y = nLength;
			break;
		case 7:
			pos = { length, length };
			break;
		default:
			break;
		}
		Physic::Query query;
		b2AABB aabb;
		pos = gp.curPos + pos;
		aabb.lowerBound = pos - boxExtent;
		aabb.upperBound = pos + boxExtent;
		auto& db = e.ECS.assign<DrawDebugComponent>(e.ECS.create());
		db.vertercies.resize(4);
		db.vertercies.emplace_back(aabb.lowerBound.x, aabb.upperBound.y);
		db.vertercies.emplace_back(aabb.upperBound);
		db.vertercies.emplace_back(aabb.upperBound.x, aabb.lowerBound.y);
		db.vertercies.emplace_back(aabb.lowerBound);
		const auto& entities = query.GetEntities(pe, aabb);
		if (ab.eEffect != entt::null)
		{
			for (auto entity : entities)
			{
				auto& te = e.ECS.assign<GES::TryAppyEffect>(entity);
				te.prefapEntity = ab.eEffect;
				te.source = ab.source;
				te.target = entity;
			}
		}
		
		e.ECS.assign<GAS::EndAbility>(e.listenerE);
	}
};