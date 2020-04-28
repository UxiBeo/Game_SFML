#pragma once
#include "System/IDrawSystem.h"
#include "System/ISystemECS.h"
class World
{
public:
	World(entt::registry& ECS);
	void Update(entt::registry& ECS);
	void Draw(Graphics& gfx, entt::registry& ECS)const;
	
	void AddNewPlayer(entt::registry& ECS);
	void AddECSSystem(std::unique_ptr<ISystemECS> newSystem);
	void AddDrawSystem(std::unique_ptr<IDrawSystem> newSystem);
	
private:
	void InitContex(entt::registry& ECS);
	void InitSystem();
private:
	std::vector<std::unique_ptr<ISystemECS>> ecsSystems;
	std::vector<std::unique_ptr<IDrawSystem>> drawSystems;
};