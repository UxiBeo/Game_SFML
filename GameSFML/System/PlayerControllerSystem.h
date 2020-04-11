#pragma once
#include "../System/ISystemECS.h"
#include "../Component/PlayerControllerComponent.h"
#include "../MaxxConsole.h"
#include "../Keyboard.h"
#include "../Mouse.h"
class PlayerControllerSystem final : public ISystemECS
{
public:
	void BeginPlay(entt::registry& ECS) final
	{
		ECS.set<PlayerControllerComponent>();
	}
	void Update(entt::registry& ECS) final
	{
		//keyboard
		auto& kbd = ECS.ctx<Keyboard>();
		sf::Vector2i dir{ 0,0 };
		if (kbd.KeyIsPressed(sf::Keyboard::A))
		{
			dir.x = -1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::D))
		{
			dir.x = 1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::S))
		{
			dir.y = -1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::W))
		{
			dir.y = 1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::Space))
		{
			if (MaxxConsole::r_showDebugPhysic == 0)
			{
				MaxxConsole::r_showDebugPhysic = 1;
			}
			else
			{
				MaxxConsole::r_showDebugPhysic = 0;
			}
		}
		
		//mouse
		auto& mouse = ECS.ctx<Mouse>();
		auto& controller = ECS.ctx<PlayerControllerComponent>();
		controller.mouseScreenPos = mouse.GetPos();
		controller.LeftIsPress = 0;
		controller.direction = dir;
		while (!mouse.IsEmpty())
		{
			auto e = mouse.Read();
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				controller.LeftIsPress++;
				continue;
			}
			/*if (e.GetType() == Mouse::Event::Type::RPress)
			{

				continue;
			}*/
		}
		
	}
};