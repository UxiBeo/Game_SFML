#pragma once
#include "ISystemECS.h"
#include "IDrawSystem.h"
class DrawGlowSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) const final;
	void BeginPlay(entt::registry& ECS) const final;
};