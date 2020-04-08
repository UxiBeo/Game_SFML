#pragma once
#include "../../System/GameplayEffectSystem.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../System/WorldTimerSystem.h"
#include "../../Component/ParentChildrenComponent.h"
void GameplayEffectSystem::Update(entt::registry& ECS)
{
	float dt = ECS.ctx<Timer::World>().dt;
	BeginEffectSystem(ECS);
	DurationSystem(ECS, dt);
	IntervalTickSystem(ECS, dt);
	StackSystem(ECS, dt);
	ExecutionSystem(ECS);
	DeleteEffectSystem(ECS);
}

void GameplayEffectSystem::BeginPlay(entt::registry& ECS)
{
	ECS.set<GES::ObserverTick>().connect(ECS, entt::collector.replace<GES::Ticks>().where<GES::TickCapacity>());
}

void GameplayEffectSystem::DeleteEffectSystem(entt::registry& ECS) const
{
	auto view = ECS.view<GES::EffectInfo, DestroyMe>();
	view.each([&ECS](auto entity, const GES::EffectInfo& eInfo, auto) {

		ECS.get_or_assign<RemoveChildrent>(eInfo.target).entties.push_back(entity);

		for (const auto& restore : eInfo.modified)
		{
			switch (restore.type)
			{
			case RPGS::Strength:
				ECS.get<RPGS::Attribute<RPGS::Strength>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::Constitution:
				ECS.get<RPGS::Attribute<RPGS::Constitution>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::Dexterity:
				ECS.get<RPGS::Attribute<RPGS::Dexterity>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::Intelligence:
				ECS.get<RPGS::Attribute<RPGS::Intelligence>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::CritChange:
				ECS.get<RPGS::Attribute<RPGS::CritChange>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::CritDame:
				ECS.get<RPGS::Attribute<RPGS::CritDame>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::Attack:
				ECS.get<RPGS::Attribute<RPGS::Attack>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::Defence:
				ECS.get<RPGS::Attribute<RPGS::Defence>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::HealthPoint:
				ECS.get<RPGS::Attribute<RPGS::HealthPoint>>(eInfo.target).value += restore.storeValue;
				break;
			case RPGS::ManaPoint:
				ECS.get<RPGS::Attribute<RPGS::ManaPoint>>(eInfo.target).value += restore.storeValue;
				break;
			default:
				break;
			}
		}
		});
}

void GameplayEffectSystem::IntervalTickSystem(entt::registry& ECS, float dt) const
{
	auto view = ECS.view<GES::IntervalTick>();
	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&ECS, dt, &view](auto entity) {
		auto& tick = view.get<GES::IntervalTick>(entity);
		tick.curTime += dt;
		GES::Ticks nTicks{0};
		while (tick.curTime >= tick.intervalTime)
		{
			tick.curTime -= tick.intervalTime;
			nTicks.value++;
		}
		if (nTicks.value > 0)
			ECS.replace<GES::Ticks>(entity, [&nTicks](auto& tick) { tick.value = nTicks.value; });
		});

	auto& obs = ECS.ctx<GES::ObserverTick>();
	for (auto& entity : obs)
	{
		auto [nTick, tickCap] = ECS.get<GES::Ticks, GES::TickCapacity>(entity);
		if (nTick.value + tickCap.curTick >= tickCap.maxTick)
		{
			nTick.value -= nTick.value + tickCap.curTick - tickCap.maxTick;
			tickCap.curTick = tickCap.maxTick;
			ECS.assign<DestroyMe>(entity);
		}
		else
		{
			tickCap.curTick += nTick.value;
		}
		ECS.assign<GES::Executions>(entity, nTick.value);
	}
}

void GameplayEffectSystem::DurationSystem(entt::registry& ECS, float dt) const
{
	ECS.view<GES::DurationLimitedTime>().each([&ECS, dt](auto entity, GES::DurationLimitedTime& duration) {
		duration.curTime += dt;
		if (duration.curTime >= duration.maxTime)
			ECS.assign<DestroyMe>(entity);
		});
	ECS.view<GES::DurationInstant>().each([&ECS, dt](auto entity, auto) {
		ECS.assign<DestroyMe>(entity);
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
				ECS.assign<DestroyMe>(entity);
				break;
			}
			sLost.curTime = sLost.maxTime;
		}
	});
}

void GameplayEffectSystem::BeginEffectSystem(entt::registry& ECS) const
{
	auto view = ECS.view<GES::BeginEffect, GES::EffectInfo>();
	std::for_each(std::execution::par, view.begin(), view.end(), [&view, &ECS](auto entity) {
		GES::EffectInfo& eInfo = view.get<GES::EffectInfo>(entity);
		//modified value
		const auto attPack = ECS.get<RPGS::AttributePack>(eInfo.target).value;
		for (size_t i = 0; i < eInfo.modified.size();)
		{
			if (eInfo.modified[i].type & attPack)
			{
				i++;
			}
			else
			{
				std::swap(eInfo.modified[i], eInfo.modified.back());
				eInfo.modified.pop_back();
			}
		}
		//capture value
		for (size_t i = 0; i < eInfo.capture.size();)
		{
			if (eInfo.capture[i].type & attPack)
			{
				i++;
			}
			else
			{
				std::swap(eInfo.capture[i], eInfo.capture.back());
				eInfo.capture.pop_back();
			}
		}
		// doing capture
		for (auto& t : eInfo.capture)
		{
			switch (t.type)
			{
			case RPGS::Attack:
				t.value = ECS.get<RPGS::Attribute<RPGS::AttributeType::Attack>>(eInfo.source).value.getFinalValue();
				break;
			default:
				break;
			}
		}
		
	});
	
}

void GameplayEffectSystem::ExecutionSystem(entt::registry& ECS) const
{
	ECS.view<GES::Executions, GES::EffectInfo>().each([&ECS, this](auto entity, const GES::Executions& eTimes, GES::EffectInfo& eInfo) {

		uint8_t totalExe = eTimes.value;
		if (auto* s = ECS.try_get<GES::Stack>(entity); s)
		{
			totalExe *= s->curStack;
		}

		if (totalExe <= 0) return;

		ModifiedAttribute(ECS, eInfo, totalExe);
		ApplyCostAndHealth(ECS, eInfo, totalExe);
		ApplyDame(ECS, eInfo, totalExe);
	});


}

void GameplayEffectSystem::TryApplyEffect(entt::registry& ECS) const
{

}

void GameplayEffectSystem::ModifiedAttribute(entt::registry& ECS, GES::EffectInfo& eInfo, uint8_t totalExe) const
{
	for (auto& m : eInfo.modified)
	{
		const auto totalV = m.modValue * totalExe;
		m.storeValue -= totalV;
		switch (m.type)
		{
		case RPGS::Strength:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::Strength>>(eInfo.target).value += totalV;
			break;
		case RPGS::Constitution:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::Constitution>>(eInfo.target).value += totalV;
			break;
		case RPGS::Dexterity:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::Dexterity>>(eInfo.target).value += totalV;
			break;
		case RPGS::Intelligence:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::Intelligence>>(eInfo.target).value += totalV;
			break;
		case RPGS::CritChange:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::CritChange>>(eInfo.target).value += totalV;
			break;
		case RPGS::CritDame:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::CritDame>>(eInfo.target).value += totalV;
			break;
		case RPGS::Attack:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::Attack>>(eInfo.target).value += totalV;
			break;
		case RPGS::Defence:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::Defence>>(eInfo.target).value += totalV;
			break;
		case RPGS::HealthPoint:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::HealthPoint>>(eInfo.target).value += totalV;
			break;
		case RPGS::ManaPoint:
			ECS.get<RPGS::Attribute<RPGS::AttributeType::ManaPoint>>(eInfo.target).value += totalV;
			break;
		default:
			assert(false && "Unhandel case");
			break;
		}
	}
}

void GameplayEffectSystem::ApplyDame(entt::registry& ECS, GES::EffectInfo& eInfo, uint8_t totalExe) const
{
	for (const auto& a : eInfo.capture)
	{
		switch (a.type)
		{
		case RPGS::Attack:
			ECS.get<RPGS::InputDame<RPGS::Attack>>(eInfo.target).value += a.value * (float)totalExe;
		break;
		default:
			break;
		}
	}
}

void GameplayEffectSystem::ApplyCostAndHealth(entt::registry& ECS, GES::EffectInfo& eInfo, uint8_t totalExe) const
{
	for (const auto& a : eInfo.costAndHealth)
	{
		switch (a.type)
		{
		case RPGS::HealthPoint:
			ECS.get<RPGS::ModifiedConsumeCur<RPGS::HealthPoint>>(eInfo.target).value += a.value * (float)totalExe;
			break;
		case RPGS::ManaPoint:
			ECS.get<RPGS::ModifiedConsumeCur<RPGS::HealthPoint>>(eInfo.target).value += a.value * (float)totalExe;
			break;
		default:
			break;
		}
	}
}
