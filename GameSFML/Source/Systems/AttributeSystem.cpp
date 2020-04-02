#pragma once

#include "../../System/AttributeSystem.h"

void AttributeSystem::Update(entt::registry& ECS)
{
	if (auto* factory = ECS.try_ctx<AttributeFactory>(); factory)
	{
		//Attribute With min max value
		{
			auto view = ECS.view<const ChangeAttribute, AttributeSet>();
			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &ECS, &factory](auto entity) {
				auto [change, atts] = view.get<const ChangeAttribute, AttributeSet>(entity);

				for (auto& i : change)
				{
					switch (i.first.value())
					{
					case Database::AHealth.value():
					{
						auto& value = ECS.get<HealthComponent>(entity);
						ChangeValueKeepPercent(*factory, i.first, atts, value.curValue,
							value.maxValue, i.second);
						break;
					}
					case Database::AMana.value():
					{
						auto& value = ECS.get<ManaComponent>(entity);
						ChangeValueKeepPercent(*factory, i.first, atts, value.curValue,
							value.maxValue, i.second);
						break;
					}
					default:
						GetAttributeByName(*factory, atts, i.first) += i.second;
						break;
					}
				}
				});
		}

		// Health
		{
			ECS.view<ChangeHealth, OnDeathNotify, HealthComponent>().each([&ECS](auto entity, const auto& amount, const auto& notify, auto& value) {

				value.curValue = std::min(unsigned int(value.curValue + amount), value.maxValue);
				if (value.curValue == 0)
				{
					ECS.remove<ChangeHealth>(entity);
					for (auto& f : notify)
						if (f) f(entity, ECS);
				}
					
				});

			auto view = ECS.view<const ChangeHealth, HealthComponent>();
			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &ECS, &factory](auto entity) {

				auto [amount, value] = view.get<const ChangeHealth, HealthComponent>(entity);

				value.curValue = std::min(unsigned int(value.curValue + amount), value.maxValue);
				});
			
			ECS.clear<ChangeHealth>();
		}
		// Mana
		{
			auto view = ECS.view<const ChangeMana, ManaComponent>();
			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &ECS, &factory](auto entity) {

				auto [amount, value] = view.get<const ChangeMana, ManaComponent>(entity);

				value.curValue = std::min(unsigned int(value.curValue + amount), value.maxValue);
				});
			ECS.clear<ChangeMana>();
		}

	}
}

