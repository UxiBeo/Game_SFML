#pragma once
#include "../AbilityComponent.h"
#include "../PhysicComponent.h"
#include "../ControllerComponent.h"
struct MeleeAttack
{
	static void OnAbilityStart(const GAS::EventInfo<GAS::Event::OnAbilityStart>& e)
	{
		e.ECS;
		e.listenerE;
		const auto& ac = e.ECS.get<GAS::AbilityComponent>(e.listenerE);
		const auto& gp = e.ECS.get<TargetPosition>(ac.source);
	}
};