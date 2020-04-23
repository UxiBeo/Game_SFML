#pragma once
#include "../System/ISystemECS.h"
#include "../MaxxConsole.h"
#include "../Keyboard.h"
#include "../Mouse.h"
#include "../Component/ControllerComponent.h"
class ControllerSystem final : public ISystemECS
{
	void BeginPlay(entt::registry& ECS) const final;
	void Update(entt::registry& ECS) const final;
	void UpdatePlayerController(entt::registry& ECS) const;
};