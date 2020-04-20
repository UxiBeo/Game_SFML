#pragma once
#include "../../System/AttributeSystem.h"
#include <vector>
void AttributeSystem::Update(entt::registry& ECS) const
{
	ApplyDame(ECS);
}


void AttributeSystem::ApplyDame(entt::registry& ECS) const
{
	auto view = ECS.view<GES::InputDame, GES::AttributePack>();
	for (auto e : view)
	{
		auto [modified, pack] = view.get<GES::InputDame, GES::AttributePack>(e);
		if (((1 << (uint32_t)GES::HealthPoint) & pack.bitmask) > 0 && modified.value != 0.0f)
		{
			auto& curValue = ECS.get<GES::SpecialValue>(pack.attribute[(uint32_t)GES::HealthPoint]).curValue;
			if (((1 << (uint32_t)GES::Defence) & pack.bitmask) > 0)
			{
				auto def = ECS.get<GES::Value>(pack.attribute[(uint32_t)GES::Defence]).getFinalValue();
				curValue -= (1 - ((0.00352f * def) / (0.993f + 0.00348f * def))) * modified.value;
			}
			else
			{
				curValue -= modified.value;
			}
		}
	}
	ECS.clear<GES::InputDame>();
}
