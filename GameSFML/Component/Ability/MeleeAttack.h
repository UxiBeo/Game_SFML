#pragma once
#include "../AbilityComponent.h"
#include "../PhysicComponent.h"
#include "../ControllerComponent.h"
#include "../AnimationComponent.h"
#include "../GameplayEffectComponent.h"
#include "../TimerComponent.h"
#include "../../Graphics.h"
#include "imgui.h"
struct MeleeAttack
{
	static void OnAbilityStart(const GAS::AbilityComponent& ab, entt::registry& ECS)
	{
		ECS.assign<GAS::CommitAbility>(ab.self);
		ECS.assign<GAS::Activating>(ab.self);
		const auto& ma = ECS.get<MeleeAttack>(ab.self);
		auto& ac = ECS.get<AnimationComponent>(ab.owner);
		auto& pm = ECS.get_or_assign<PlayAnimationMontage>(ab.owner);
		pm.an.triggerTime = ma.triggerTime;
		pm.an.listenerE = ab.self;
		pm.an.triggerD.connect<&MeleeAttack::OnAnimationNotify>();
		pm.an.interrupD.connect<&MeleeAttack::OnAnimationInterrupt>();
		pm.iSet = ac.iSet % 4 + 4 * ma.aSet;
	}
	static void ShowMeleeWindow(entt::registry& ECS)
	{
		if (ImGui::Begin("Meelee Attack"))
		{
			float min = -10.0f;
			float max = 10.0f;
			ECS.view<MeleeAttack>().each([&min, &max](auto entity, MeleeAttack& ma) {
				ImGui::SliderFloat("aoeH.X", &ma.aoeH.x, min, max, "%.1f");
				ImGui::SliderFloat("aoeH.y", &ma.aoeH.y, min, max, "%.1f");
				ImGui::SliderFloat("aoeV.y", &ma.aoeV.y, min, max, "%.1f");
				ImGui::SliderFloat("aoeV.x", &ma.aoeV.x, min, max, "%.1f");
				ImGui::SliderFloat("attackOffsetD.x", &ma.attackOffsetD.x, min, max, "%.1f");
				ImGui::SliderFloat("attackOffsetD.y", &ma.attackOffsetD.y, min, max, "%.1f");
				ImGui::SliderFloat("attackOffsetU.x", &ma.attackOffsetU.x, min, max, "%.1f");
				ImGui::SliderFloat("attackOffsetU.y", &ma.attackOffsetU.y, min, max, "%.1f");
				});
		}
		ImGui::End();
	}
private:
	static void OnAnimationInterrupt(const entt::entity& owner, const entt::entity& self, entt::registry& ECS)
	{
		ECS.remove<GAS::Activating>(self);
	}
	static void OnAnimationNotify(const entt::entity& owner, const entt::entity& self, entt::registry& ECS)
	{
		auto& pe = ECS.ctx<Physic::Engine>();
		const auto& gp = ECS.get<TargetPosition>(owner);
		const auto& ac = ECS.get<AnimationComponent>(owner);
		const auto& me = ECS.get<MeleeAttack>(self);
		float nLength = 4.0f;
		float length = nLength / sqrt(2.0f);
		b2Vec2 boxExtent{ 0.0f,0.0f };
		b2Vec2 pos{ 0.0f,0.0f };
		switch (ac.iSet % 4)
		{
		case 0:
		{
			pos = me.attackOffsetLR;
			boxExtent = me.aoeV;
		}
			break;
		case 1:
		{
			pos = me.attackOffsetD;
			boxExtent = me.aoeH;
		}
			break;
		case 2:
		{
			pos = { -me.attackOffsetLR.x, me.attackOffsetLR.y };
			boxExtent = me.aoeV;
		}
			break;
		case 3:
		{
			pos = me.attackOffsetU;
			boxExtent = me.aoeH;
		}
			break;
		default:
			break;
		}
		
		b2AABB aabb;
		pos += gp.curPos;
		aabb.lowerBound = pos - boxExtent;
		aabb.upperBound = pos + boxExtent;
		{
			auto debugEntity = ECS.create();
			ECS.assign<Timer::LifeTimeComponent>(debugEntity, 0.0f, 5.0f);
			auto& rect = ECS.assign<sf::RectangleShape>(debugEntity);
			rect.setSize(Graphics::WorldToScreenPos(2.0f * boxExtent));
			rect.setOrigin(Graphics::WorldToScreenPos(boxExtent));
			rect.setPosition(Graphics::WorldToScreenPos(pos));
			rect.setFillColor(sf::Color(150, 50, 150, 128));
		}
		

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
public:
	uint8_t aSet = 1;
	float triggerTime = 0.0f;
	b2Vec2 attackOffsetLR = {4.0f, 4.0f};
	b2Vec2 attackOffsetD = { 0.0f, -2.2f };
	b2Vec2 attackOffsetU = { 0.0f, 7.0f };
	b2Vec2 aoeV = { 3.0f,3.0f };
	b2Vec2 aoeH = { 3.0f, 2.0f };
};