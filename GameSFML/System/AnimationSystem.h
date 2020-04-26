#pragma once
#include "../System/ISystemECS.h"
class AnimationSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final;
	void UpdateAnimationNotify(entt::registry& ECS, const float dt) const;
	void UpdateAniamtionMontage(entt::registry& ECS, const float dt) const;
};