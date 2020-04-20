#pragma once
#include "../System/ISystemECS.h"
class AnimationSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final;
};