#pragma once

#include "../System/ISystemECS.h"
class GameplayEffectSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final;
	
};