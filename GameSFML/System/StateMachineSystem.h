#pragma once
#include "../System/ISystemECS.h"
class StateMachineSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final;
};