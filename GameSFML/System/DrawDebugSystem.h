#pragma once
#include "../Graphics.h"
#include "../Component/PhysicComponent.h"
#include "../System/IDrawSystem.h"
#include "../System/ISystemECS.h"
#include "../MaxxConsole.h"
#include "../Component/TimerComponent.h"
class DrawDebugSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::registry& ECS) const final
	{
		ECS.view<Physic::Component, sf::RectangleShape>().each([&gfx](auto enity, Physic::Component& pc, sf::RectangleShape& cs) {
			cs.setPosition(gfx.WorldToScreenPos(pc->GetPosition()));
			});
		ECS.view<sf::RectangleShape>().each([&gfx](auto enity, sf::RectangleShape& cs) {
			gfx.Draw(cs);
			});
	}
	
};