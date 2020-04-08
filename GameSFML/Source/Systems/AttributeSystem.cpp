#pragma once
#include "../../System/AttributeSystem.h"
#include <vector>
void AttributeSystem::Update(entt::registry& ECS)
{
	// base
	ModifiedAttribute<ATT::Constitution>(ECS);
	ModifiedAttribute<ATT::Dexterity   >(ECS);
	ModifiedAttribute<ATT::Intelligence>(ECS);
	ModifiedAttribute<ATT::Strength    >(ECS);
	
	//dependent
	ModifiedAttribute<ATT::Attack      >(ECS);
	ModifiedAttribute<ATT::CritChange  >(ECS);
	ModifiedAttribute<ATT::CritDame    >(ECS);
	ModifiedAttribute<ATT::Defence     >(ECS);

	//consume
	ModifiedAttribute<ATT::HealthPoint >(ECS);
	ModifiedAttribute<ATT::ManaPoint   >(ECS);

	ModifiedConsumeCurrent(ECS);
	ApplyDame(ECS);
	ClearModified(ECS);
}

void AttributeSystem::ClearModified(entt::registry& ECS) const
{
	ECS.clear<RPGS::ModifiedAttribute<ATT::Constitution>>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::Dexterity   >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::Intelligence>>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::Strength    >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::Attack      >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::CritChange  >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::CritDame    >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::Defence     >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::HealthPoint >>();
	ECS.clear<RPGS::ModifiedAttribute<ATT::ManaPoint   >>();
	ECS.clear<RPGS::ModifiedConsumeCur<ATT::HealthPoint >>();
	ECS.clear<RPGS::ModifiedConsumeCur<ATT::ManaPoint   >>();
}

void AttributeSystem::ModifiedConsumeCurrent(entt::registry& ECS) const
{
	auto viewH = ECS.view<const RPGS::ModifiedConsumeCur<ATT::HealthPoint>, RPGS::Attribute<ATT::HealthPoint>>();
	std::for_each(std::execution::par_unseq, viewH.begin(), viewH.end(), [&viewH](auto entity) {
		auto [modV, cVal] = viewH.get<const RPGS::ModifiedConsumeCur<ATT::HealthPoint>, RPGS::Attribute<ATT::HealthPoint>>(entity);
		cVal.value += modV.value;

		});
	auto viewM = ECS.view<const RPGS::ModifiedConsumeCur<ATT::ManaPoint>, RPGS::Attribute<ATT::ManaPoint>>();
	std::for_each(std::execution::par_unseq, viewM.begin(), viewM.end(), [&viewM](auto entity) {
		auto [modV, cVal] = viewM.get<const RPGS::ModifiedConsumeCur<ATT::ManaPoint>, RPGS::Attribute<ATT::ManaPoint>>(entity);
		cVal.value += modV.value;

		});

}

void AttributeSystem::ApplyDame(entt::registry& ECS) const
{
	ECS.group<RPGS::InputDame<RPGS::Attack>, RPGS::Attribute<RPGS::Defence>, RPGS::Attribute<RPGS::HealthPoint>>().each(
		[](auto entity, const RPGS::InputDame<RPGS::Attack>& atk, const RPGS::Attribute<RPGS::Defence>& def, RPGS::Attribute<RPGS::HealthPoint>& hp) {
			auto dValue = def.value.getFinalValue();
			hp.value.curValue -= atk.value * (1.0f - ((0.00352f * dValue) / (0.993f + 0.00348f * dValue)));
		});
}
