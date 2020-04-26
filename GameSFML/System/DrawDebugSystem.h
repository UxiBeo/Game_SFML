#pragma once
#include "../Graphics.h"
#include "../Component/PhysicComponent.h"
#include "../System/IDrawSystem.h"
#include "../System/ISystemECS.h"
#include "../MaxxConsole.h"
#include "../Component/DrawDebugComponent.h"
#include "../Component/TimerComponent.h"
class DrawDebugSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::registry& ECS) const final
	{
		ECS.view<Physic::Component, sf::CircleShape>().each([&gfx](auto enity, Physic::Component& pc, sf::CircleShape& cs) {
			cs.setPosition(gfx.WorldToScreenPos(pc->GetPosition()));
			gfx.Draw(cs);
			});
		auto& vertices = ECS.ctx<DebugEntities>();
		vertices.vArray.clear();
		ECS.view<DrawDebugComponent>().each([&ECS, &vertices,&gfx](auto entity, DrawDebugComponent& dc) {
			
			for (auto& v : dc.vertices)
			{
				vertices.vArray.append({gfx.WorldToScreenPos(v), dc.color});
			}
			
			});
		gfx.Draw(vertices);
	}
	
};

class DrawDebugUpdate final : public ISystemECS
{
	void Update(entt::registry& ECS) const final
	{
		const auto dt = ECS.ctx<Timer::World>().dt;		
		ECS.view<DrawDebugComponent>().each([&ECS, dt](auto entity, DrawDebugComponent& dc) {
			dc.dt += dt;
			if (dc.dt > 5.0f)
			{
				ECS.destroy(entity);
			}
			});
	}
	void BeginPlay(entt::registry& ECS) const final
	{
		ECS.set<DebugEntities>().vArray.setPrimitiveType(sf::Quads);
	}
};