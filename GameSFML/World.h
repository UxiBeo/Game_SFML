#pragma once
#include "System/IDrawSystem.h"
#include "System/ISystemECS.h"
class World
{
public:
	World();
	void Update();
	void Draw()const;
	
	void AddNewPlayer(entt::registry& ECS);
	void AddECSSystem(std::unique_ptr<ISystemECS> newSystem)
	{
		ecsSystems.emplace_back(std::move(newSystem));
	}
	void AddDrawSystem(std::unique_ptr<IDrawSystem> newSystem)
	{
		drawSystems.emplace_back(std::move(newSystem));
	}
	
	void AddPlayer(entt::registry& ECS);
	
private:
	void BeginPlay(entt::registry& ECS);
	
	void InitServiceLocator();
	
	void InitSystem();
	
	void AddWall(b2Vec2 p1, b2Vec2 p2);
	
	void TestSomething();
	
private:
	std::vector<std::unique_ptr<ISystemECS>> ecsSystems;
	std::vector<std::unique_ptr<IDrawSystem>> drawSystems;
};