#pragma once
#include "../System/ISystemECS.h"
#include "../System/IDrawSystem.h"
class SpirteUpdateSystem final : public ISystemECS
{
public:
	//update before Culling, animation, physic
	void Update(entt::registry& ECS)const final;
	void BeginPlay(entt::registry& ECS) const final;
};
class RenderSpriteSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::registry& ECS) const final;
};