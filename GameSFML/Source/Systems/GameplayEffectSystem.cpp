#pragma once
#include <cassert>
#include "../../System/GameplayEffectSystem.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../System/WorldTimerSystem.h"
#include "../../Component/ParentChildrenComponent.h"
#include "../../Component/StampContex.h"
void GameplayEffectSystem::Update(entt::registry& ECS)const
{
	float dt = ECS.ctx<Timer::World>().dt;
	TryApplyEffect(ECS);
	RenewEffectSystem(ECS);
	DurationSystem(ECS, dt);
	IntervalTickSystem(ECS, dt);
	StackSystem(ECS, dt);
	ExecutionSystem(ECS);
	DeleteEffect(ECS);
}

void GameplayEffectSystem::BeginPlay(entt::registry& ECS)const
{
	ECS.on_destroy<GES::CurrentActiveEffect>().connect<&GameplayEffectSystem::OnDestroyedCurrentEffect>();
	auto& sCtx = ECS.ctx<StampContex>();
	sCtx.AddStampFunction<GES::AddStack>();
	sCtx.AddStampFunction<GES::DurationPassive>();
	sCtx.AddStampFunction<GES::DurationOnetime>();
	sCtx.AddStampFunction<GES::DurationLimitedTime>();
	sCtx.AddStampFunction<GES::IntervalTick>();
	sCtx.AddStampFunction<GES::Stack>();
	sCtx.AddStampFunction<GES::StackLost>();
	sCtx.AddStampFunction<GES::ModifiedAttribute>();
	sCtx.AddStampFunction<GES::ModifiedCurrentSpecial>();
	sCtx.AddStampFunction<GES::OutputDame>();
	sCtx.AddStampFunction<GES::EffectInfo>();
}

void GameplayEffectSystem::OnDestroyedCurrentEffect(entt::registry& ECS, entt::entity entity)
{
	for (auto& e : ECS.get<GES::CurrentActiveEffect>(entity).entities)
	{
		ECS.destroy(e.second);
	}
}

void GameplayEffectSystem::DeleteEffect(entt::registry& ECS) const
{
	RestoreAttribute(ECS);

	auto view = ECS.view<GES::MarkDelete, GES::EffectInfo>();
	view.each([&ECS](auto entity, auto, const GES::EffectInfo& ei) {

		auto& cEffect = ECS.get<GES::CurrentActiveEffect>(ei.target);
		assert(cEffect.entities.count(ei.prefapEntity) > 0);
		cEffect.entities.erase(ei.prefapEntity);
		auto& targetTag = ECS.get<Tag::Bitfiled>(ei.target);
		targetTag |= ei.tags.end_target_GrantTags;
		targetTag &= ~ei.tags.end_target_RemoveTags;

		});
	ECS.destroy(view.begin(), view.end());
}

void GameplayEffectSystem::IntervalTickSystem(entt::registry& ECS, float dt) const
{
	ECS.view<GES::IntervalTick>().each([&ECS, dt](auto entity, GES::IntervalTick& it) {
		it.curTime += dt;
		uint8_t nTicks = 0;
		while (it.curTime >= it.intervalTime)
		{
			it.curTime -= it.intervalTime;
			nTicks++;
		}
		if (nTicks == 0) return;

		if (it.maxTick > 0)
		{
			if (nTicks + it.curTick >= it.maxTick)
			{
				nTicks -= nTicks + it.curTick - it.maxTick;
				ECS.assign<GES::MarkDelete>(entity);
			}
			else
			{
				it.curTick += nTicks;
			}
		}
		if (nTicks > 0)
			ECS.get_or_assign<GES::Executions>(entity).value += nTicks;
		});
}

void GameplayEffectSystem::DurationSystem(entt::registry& ECS, float dt) const
{
	ECS.view<GES::DurationLimitedTime>().each([&ECS, dt](auto entity, GES::DurationLimitedTime& duration) {
		duration.curTime += dt;
		if (duration.curTime >= duration.maxTime)
			ECS.assign<GES::MarkDelete>(entity);
		});
	ECS.view<GES::DurationOnetime>().each([&ECS, dt](auto entity, auto) {
		ECS.assign<GES::MarkDelete>(entity);
		ECS.get_or_assign<GES::Executions>(entity).value += (uint8_t)1;
		});
}

void GameplayEffectSystem::StackSystem(entt::registry& ECS, float dt) const
{
	{
		auto view = ECS.view<GES::AddStack, GES::Stack>();
		std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&ECS, &view](auto entity) {
			auto [add, stack] = view.get<GES::AddStack, GES::Stack>(entity);
			stack.curStack += add.value;

			if (stack.curStack > stack.maxStack)
			{
				if (auto* sLost = ECS.try_get<GES::StackLost>(entity); sLost)
				{
					sLost->curTime = sLost->maxTime;
				}
			}
			});
	}
	

	ECS.view<GES::StackLost, GES::Stack>().each([&ECS, dt](auto entity, GES::StackLost& sLost, GES::Stack& stack) {
		sLost.curTime -= dt;
		while (sLost.curTime <= 0.0f)
		{
			stack.curStack--;
			stack.curStack = std::max<uint8_t>(stack.curStack - (uint8_t)1, stack.minStack);
			if (stack.curStack == 0)
			{
				ECS.assign<GES::MarkDelete>(entity);
				break;
			}
			sLost.curTime = sLost.maxTime;
		}
	});
}

void GameplayEffectSystem::ExecutionSystem(entt::registry& ECS) const
{
	ECS.view<GES::Executions, GES::Stack>().each([&ECS](auto entity,GES::Executions& ec, const GES::Stack& sc) {
		ec.value *= sc.curStack;
		if (ec.value == 0)
			ECS.remove<GES::Executions>(entity);
	});

	ModifiedAttribute(ECS);
	ModifiedCurrentSpecial(ECS);
	ApplyDame(ECS);
	ECS.clear<GES::Executions>();
}

void GameplayEffectSystem::TryApplyEffect(entt::registry& ECS) const
{
	auto view = ECS.view<GES::TryAppyEffect>();
	if (view.size() == 0) return;
	auto& stamp = ECS.ctx<StampContex>();
	const auto& prefap = ECS.ctx<PrefapRegistry>();
	for (auto entity : view)
	{
		const auto& te = view.get<GES::TryAppyEffect>(entity);
		assert(te.prefapEntity != entt::null);
		assert(te.target != entt::null);
		assert(te.source != entt::null);

		auto& cEffect = ECS.get<GES::CurrentActiveEffect>(te.target);
		if (cEffect.entities.count(te.prefapEntity) > 0)
		{
			auto& ne = ECS.get_or_assign<GES::RenewEffect>(cEffect.entities[te.prefapEntity]);
			++ne.times;
			if (ne.entity != entt::null)
			{
				ECS.destroy(ne.entity);
			}
			ne.entity = entity;
			return;
		}

		stamp.Clone(prefap.ECS, te.prefapEntity, ECS, entity);
		auto& ei = ECS.get<GES::EffectInfo>(entity);
		auto& targetTag = ECS.get<Tag::Bitfiled>(te.target);
		targetTag |= ei.tags.begin_target_GrantTags;
		targetTag &= ~ei.tags.begin_target_RemoveTags;
		ei.target = te.target;
		ei.source = te.source;
		ei.prefapEntity = te.prefapEntity;
	}
	ECS.clear<GES::TryAppyEffect>();
	view.each([&ECS](auto entity, const GES::TryAppyEffect& te) {
		


		/*auto& targetTag = ECS.get<Tag::Bitfiled>(te.target);
		const GES::EffectResource& ei = Codex<GES::EffectResource>::Retrieve(te.effectName);
		if (ei.tags.target_RequiredTags == (ei.tags.target_RequiredTags & targetTag) && (ei.tags.target_BlockTags & targetTag) > 0)
		{
			targetTag |= ei.tags.begin_target_GrantTags;
			targetTag &= ~ei.tags.begin_target_RemoveTags;

			switch (ei.durationType)
			{
			case GES::DurationType::Passive:
				ECS.assign<GES::DurationPassive>(entity);
				break;
			case GES::DurationType::Onetime:
				ECS.assign<GES::DurationOnetime>(entity);
				break;
			case GES::DurationType::LimitedTime:
				ECS.assign<GES::DurationLimitedTime>(entity).maxTime = ei.durationTime;
				break;
			default:
				break;
			}

			if (ei.intervalTick > 0.0f && ei.maxTick > 0)
			{
				auto& interval = ECS.assign<GES::IntervalTick>(entity);
				interval.intervalTime = ei.intervalTick;
				interval.maxTick = ei.maxTick;
			}

			for (const auto& i : ei.CostAmount)
			{
				if (i.first == RPGS::AttributeType::HealthPoint)
				{
					auto& value = ECS.assign<GES::CostValue<RPGS::AttributeType::HealthPoint>>(entity);
					value.value = i.second;
					value.target = te.target;
				}
				else
				{
					auto& value = ECS.assign<GES::CostValue<RPGS::AttributeType::ManaPoint>>(entity);
					value.value = i.second;
					value.target = te.target;
				}
			}
				
		}*/
		});
}

void GameplayEffectSystem::ApplyDame(entt::registry& ECS) const
{
	ECS.view<GES::Executions, GES::OutputDame, GES::EffectInfo>().each([&ECS](auto entity, const GES::Executions& ec, GES::OutputDame& od, const GES::EffectInfo& ei) {
		ECS.get_or_assign<GES::InputDame>(ei.target).value += od.value * ec.value;
		});
}

void GameplayEffectSystem::ModifiedAttribute(entt::registry& ECS) const
{
	ECS.view<GES::Executions, GES::ModifiedAttribute, GES::EffectInfo>().each([&ECS](auto entity, const GES::Executions& ec, GES::ModifiedAttribute& ma, const GES::EffectInfo& ei) {
		auto& pack = ECS.get<GES::AttributePack>(ei.target);
		for (auto& p : ma.value)
		{
			if ((pack.bitmask & (1 << (unsigned int)p.type))  > 0)
			{
				auto value = p.modValue * (float)ec.value;
				p.storeValue -= value;
				switch (p.type)
				{
				case GES::CritChange:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::CritChange]) += value;
					break;
				case GES::CritDame:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::CritDame]) += value;
					break;
				case GES::Attack:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::Attack]) += value;
					break;
				case GES::Defence:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::Defence]) += value;
					break;
				case GES::HealthPoint:
					ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::HealthPoint]) += value;
					break;
				case GES::ManaPoint:
					ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::ManaPoint]) += value;
					break;
				}
			}
		}
	});

}

void GameplayEffectSystem::ModifiedCurrentSpecial(entt::registry& ECS) const
{
	ECS.view<GES::Executions, GES::ModifiedCurrentSpecial, GES::EffectInfo>().each([&ECS](auto entity, const GES::Executions& ec, GES::ModifiedCurrentSpecial& ms, const GES::EffectInfo& ei) {
		auto& pack = ECS.get<GES::AttributePack>(ei.target);
		if ((pack.bitmask & (1 << (uint32_t)GES::HealthPoint)) > 0 && ms.value[0] != 0.0f)
			ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::HealthPoint]) += ms.value[0];

		if ((pack.bitmask & (1 << (uint32_t)GES::ManaPoint)) > 0 && ms.value[1] != 0.0f)
			ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::ManaPoint]) += ms.value[1];
		});
	
}

void GameplayEffectSystem::RestoreAttribute(entt::registry& ECS) const
{
	ECS.view<GES::MarkDelete, const GES::ModifiedAttribute, const GES::EffectInfo>().each([&ECS](auto entity, auto, const GES::ModifiedAttribute& ma, const GES::EffectInfo& ei) {
		auto& pack = ECS.get< GES::AttributePack>(ei.target);
		for (auto& p : ma.value)
		{
			if (((1 << p.type) & pack.bitmask) > 0)
			{
				switch (p.type)
				{
				case GES::CritChange:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::CritChange]) -= p.storeValue;
					break;
				case GES::CritDame:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::CritDame]) -= p.storeValue;
					break;
				case GES::Attack:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::Attack]) -= p.storeValue;
					break;
				case GES::Defence:
					ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::Defence]) -= p.storeValue;
					break;
				case GES::HealthPoint:
					ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::HealthPoint]) -= p.storeValue;
					break;
				case GES::ManaPoint:
					ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::ManaPoint]) -= p.storeValue;
					break;
				}
			}
		}
		});
}

void GameplayEffectSystem::RenewEffectSystem(entt::registry& ECS) const
{
	ECS.view<GES::RenewEffect, GES::EffectInfo>();
	ECS.clear<GES::RenewEffect>();
}
