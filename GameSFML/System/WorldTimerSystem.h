#pragma once
#include "../System/ISystemECS.h"
#include "../Component/TimerComponent.h"
#include <optional>
class WorldTimerSystem final : public ISystemECS
{
public:
	void BeginPlay(entt::registry& ECS)const  final;
	void Update(entt::registry& ECS) const final;
};