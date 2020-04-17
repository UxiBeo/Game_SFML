#pragma once
#include "../../System/GameplayEffectSystem.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../System/WorldTimerSystem.h"
#include "../../Component/ParentChildrenComponent.h"
void GameplayEffectSystem::Update(entt::registry& ECS)
{
	float dt = ECS.ctx<Timer::World>().dt;
	TryApplyEffect(ECS);
	DurationSystem(ECS, dt);
	IntervalTickSystem(ECS, dt);
	StackSystem(ECS, dt);
	ExecutionSystem(ECS);
	DeleteEffect(ECS);
}

void GameplayEffectSystem::BeginPlay(entt::registry& ECS)
{
	
}						



void GameplayEffectSystem::DeleteEffect(entt::registry& ECS) const
{
	RestoreAttribute<RPGS::Attack		>(ECS);
	RestoreAttribute<RPGS::CritChange	>(ECS);
	RestoreAttribute<RPGS::CritDame		>(ECS);
	RestoreAttribute<RPGS::Defence		>(ECS);
	RestoreAttribute<RPGS::HealthPoint	>(ECS);
	RestoreAttribute<RPGS::ManaPoint	>(ECS);

	auto view = ECS.view<GES::MarkDelete, GES::EffectInfo>();
	view.each([&ECS](auto entity, auto, const GES::EffectInfo& ei) {
		ECS.get<GES::CurrentActiveEffect>(ei.target).entities.erase(entity);
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
			ECS.assign<GES::Executions>(entity, nTicks);
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
		ECS.assign<GES::Executions>(entity, (uint8_t)1);
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

	ModifiedAttribute<RPGS::Attack		>(ECS);
	ModifiedAttribute<RPGS::CritChange	>(ECS);
	ModifiedAttribute<RPGS::CritDame	>(ECS);
	ModifiedAttribute<RPGS::Defence		>(ECS);
	ModifiedAttribute<RPGS::HealthPoint	>(ECS);
	ModifiedAttribute<RPGS::ManaPoint	>(ECS);
	ApplyCost<RPGS::ManaPoint>(ECS);
	ApplyCost<RPGS::HealthPoint>(ECS);
	ApplyDame<RPGS::Attack>(ECS);
}

void GameplayEffectSystem::TryApplyEffect(entt::registry& ECS) const
{
	ECS.view<GES::TryAppyEffect>().each([&ECS](auto entity, const GES::TryAppyEffect& te) {
		const GES::EffectResource& ei = Codex<GES::EffectResource>::Retrieve(te.effectName);

		auto& targetTag = ECS.get<Tag::Bitfiled>(te.target);
		auto& cEffect = ECS.get<GES::CurrentActiveEffect>(te.target);
		entt::entity anotherEffect= entt::null;
		for (auto& i : cEffect.entities)
		{
			if (te.effectName == ECS.get<GES::EffectInfo>(i).effectName)
			{
				anotherEffect = i;
				break;
			}
		}
		if (ei.tags.target_RequiredTags == (ei.tags.target_RequiredTags & targetTag) && (ei.tags.target_BlockTags & targetTag) > 0)
		{
			if (ei.minStack = 1)
			{
				ECS.assign<GES::AddStack>(i);

			}
			else
			{
				targetTag |= ei.tags.begin_target_GrantTags;
				targetTag &= ~ei.tags.begin_target_RemoveTags;

				switch (type)
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
			}
		}
		});

	CaptureAttack<RPGS::Attack>(ECS);
}
