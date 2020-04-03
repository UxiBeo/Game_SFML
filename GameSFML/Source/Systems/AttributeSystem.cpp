#pragma once
#include "../../System/AttributeSystem.h"

void AttributeSystem::Update(entt::registry& ECS)
{
	ChangeValueSystem<STATS::Base, STATS::ChangeBase>(ECS);
	ChangeValueSystem<STATS::Depent, STATS::ChangeDepent>(ECS);
	{
		auto view = ECS.view<const STATS::ChangeSpecial, STATS::SpecialSet>();
		std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
			auto [change, atts] = view.get<const STATS::ChangeSpecial, STATS::SpecialSet>(entity);
			for (auto& i : change)
			{
				atts[i.first].curValue += (int)i.second;
			}

			});
	}
}

void AttributeSystem::BeginPlay(entt::registry& ECS)
{
	auto factory = ECS.set<AttributeFactory>();
	ECS.group<STATS::Base, const STATS::ChangeBase>();
	ECS.group<STATS::Depent, const STATS::ChangeDepent>();
}
