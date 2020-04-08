#define AttXMacro
#include "../../System/GameplayEffectSystem.h"
#include "../../Component/GameplayEffectComponent.h"
#include "../../System/WorldTimerSystem.h"
void GameplayEffectSystem::Update(entt::registry& ECS)
{
	float dt = ECS.ctx<WorldTimer>().dt;
	BeginEffectSystem(ECS);
	DurationSystem(ECS, dt);
	IntervalTickSystem(ECS, dt);
	StackSystem(ECS, dt);
	ExcutionSystem(ECS);
	DeleteEffectSystem(ECS);
}

void GameplayEffectSystem::BeginPlay(entt::registry& ECS)
{
	ECS.set<GAS::ObserverTick>().connect(ECS, entt::collector.replace<GAS::TickTimes>().where<GAS::TickCapacity>());
}

void GameplayEffectSystem::DeleteEffectSystem(entt::registry& ECS) const
{
	#define X(el) ECS.view<GAS::DeleteEffect, GAS::RestoreAttribute<RPGS::AttributeType::el>, GAS::EffectInfo>().each([&ECS](auto entity, const auto& rAtt, const GAS::EffectInfo& eInfo) {\
		auto& attValue = ECS.get<RPGS::Attribute<RPGS::AttributeType::el>>(eInfo.target).value;\
		attValue += rAtt.value; });
		LEAF_ELEMENT_TYPES
	#undef X
	
	auto deleveView = ECS.view <GAS::DeleteEffect>();
	ECS.destroy(deleveView.begin(), deleveView.end());
}

void GameplayEffectSystem::IntervalTickSystem(entt::registry& ECS, float dt) const
{
	auto view = ECS.view<GAS::IntervalTick>();
	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&ECS, dt, &view](auto entity) {
		auto& tick = view.get<GAS::IntervalTick>(entity);
		tick.curTime += dt;
		GAS::TickTimes nTicks = 0;
		while (tick.curTime >= tick.intervalTime)
		{
			tick.curTime -= tick.intervalTime;
			nTicks++;
		}
		if (nTicks > 0)
			ECS.replace<GAS::TickTimes>(entity, [&nTicks](auto& tick) { tick = nTicks; });
		});

	auto& obs = ECS.ctx<GAS::ObserverTick>();
	for (auto& entity : obs)
	{
		auto [nTick, tickCap] = ECS.get<GAS::TickTimes, GAS::TickCapacity>(entity);
		if (nTick + tickCap.curTick >= tickCap.maxTick)
		{
			nTick -= nTick + tickCap.curTick - tickCap.maxTick;
			tickCap.curTick = tickCap.maxTick;
			ECS.assign<GAS::DeleteEffect>(entity);
		}
		else
		{
			tickCap.curTick += nTick;
		}
		ECS.assign<GAS::ExecutionTimes>(entity, nTick);
	}
}

void GameplayEffectSystem::DurationSystem(entt::registry& ECS, float dt) const
{
	ECS.view<GAS::DurationLimitedTime>().each([&ECS, dt](auto entity, GAS::DurationLimitedTime& duration) {
		duration.curTime += dt;
		if (duration.curTime >= duration.maxTime)
			ECS.assign<GAS::DeleteEffect>(entity);
		});
	ECS.view<GAS::DurationInstant>().each([&ECS, dt](auto entity, auto) {
		ECS.assign<GAS::DeleteEffect>(entity);
		ECS.assign<GAS::ExecutionTimes>(entity, 1u);
		});
}

void GameplayEffectSystem::StackSystem(entt::registry& ECS, float dt) const
{
	{
		auto view = ECS.view<GAS::AddStack, GAS::Stack>();
		std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&ECS, &view](auto entity) {
			auto [add, stack] = view.get<GAS::AddStack, GAS::Stack>(entity);
			stack.curStack += add;

			if (stack.curStack > stack.maxStack)
			{
				if (auto* sLost = ECS.try_get<GAS::StackLost>(entity); sLost)
				{
					sLost->curTime = sLost->maxTime;
				}
			}
			});
	}
	

	ECS.view<GAS::StackLost, GAS::Stack>().each([&ECS, dt](auto entity, GAS::StackLost& sLost, GAS::Stack& stack) {
		sLost.curTime -= dt;
		while (sLost.curTime <= 0.0f)
		{
			stack.curStack--;
			if (stack.curStack == 0)
			{
				ECS.assign<GAS::DeleteEffect>(entity);
				break;
			}
			sLost.curTime = sLost.maxTime;
		}
	});
}

void GameplayEffectSystem::BeginEffectSystem(entt::registry& ECS) const
{
	ECS.view<GAS::BeginEffect, GAS::EffectInfo>().each([&](auto entity, GAS::EffectInfo& eInfo) {
		std::for_each(std::execution::par_unseq, eInfo.captureAtts.begin(), eInfo.captureAtts.end(), [&](std::pair<RPGS::AttributeType, float>& type) {
			switch (type.first)
			{
				#define X(el) case RPGS::AttributeType::el: \
				{\
					if (const auto* cap = ECS.try_get<RPGS::Attribute<RPGS::AttributeType::el>>(eInfo.source); cap) \
					{\
						type.second = cap->value.getFinalValue(); \
					}\
					break;\
				}
				LEAF_ELEMENT_TYPES
				#undef X
			default:
				assert("Unhandle Case" && false);
				break;
			}
		});

		eInfo.captureAtts.erase(std::remove_if(eInfo.captureAtts.begin(), eInfo.captureAtts.end(),
			[](std::pair<RPGS::AttributeType, float>& value) { return (int)value.second == 0; }), eInfo.captureAtts.end());

	});
	
}

void GameplayEffectSystem::ExecutionSystem(entt::registry& ECS) const
{
	ECS.view<GAS::ExecutionTimes, GAS::EffectInfo>().each([&ECS](auto entity, const GAS::ExecutionTimes& eTimes, GAS::EffectInfo& eInfo) {
		
		std::for_each(std::execution::par_unseq, eInfo.modifiedAtts.begin(), eInfo.modifiedAtts.end(), [&](const std::pair<RPGS::AttributeType, RPGS::Value>& type) {
			switch (type.first)
			{
			#define X(el) case RPGS::AttributeType::el: \
				{\
					if (auto* cap = ECS.try_get<RPGS::Attribute<RPGS::AttributeType::el>>(eInfo.target); cap) \
					{\
						auto modValue = type.second * (float)eTimes.data;\
						cap->value += modValue;\
						ECS.get_or_assign<GAS::RestoreAttribute<RPGS::AttributeType::el>>(entity).value -= modValue;\
					} \
					break;\
				}
				LEAF_ELEMENT_TYPES
			#undef X
			default:
				assert("Unhandle Case" && false);
				break;
			}
		});
		if (eInfo.firstTime)
		{
			eInfo.firstTime = false;
			eInfo.modifiedAtts.erase(std::remove_if(eInfo.modifiedAtts.begin(), eInfo.modifiedAtts.end(),
				[](std::pair<RPGS::AttributeType, RPGS::Value>& value) { return value.second.isZero(); }), eInfo.modifiedAtts.end());
		}

		std::for_each(std::execution::par_unseq, eInfo.captureAtts.begin(), eInfo.captureAtts.end(), [&](const std::pair<RPGS::AttributeType, float>& cAtt) {
			
			
			});
		
	});


}

void GameplayEffectSystem::TryApplyEffect(entt::registry& ECS) const
{

}
