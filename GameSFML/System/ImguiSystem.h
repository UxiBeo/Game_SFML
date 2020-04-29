#pragma once
#include "ISystemECS.h"
#include "IDrawSystem.h"
class ImguiUpdateSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) const final;
private:
	void ShowMeleeWindow(entt::registry& ECS) const;
	void ShowProjectTileWindow(entt::registry& ECS) const;
};
class ImguiDrawSystem final : public IDrawSystem
{
	void Draw(Graphics& gfx, entt::registry& ECS) const final;
};