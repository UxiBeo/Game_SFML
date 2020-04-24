#pragma once
#include "../Graphics.h"
#include "../Component/PhysicComponent.h"
#include "../System/IDrawSystem.h"
#include "../System/ISystemECS.h"
#include "../MaxxConsole.h"
#include "../DrawDebugComponent.h"
#include "../Component/TimerComponent.h"
class DrawDebugSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::registry& ECS) const final
	{
		auto& vertercies = ECS.ctx<DebugEntities>();
		vertercies.vArray.clear();
		ECS.view<DrawDebugComponent>().each([&ECS, &vertercies,&gfx](auto entity, DrawDebugComponent& dc) {
			
			for (auto& v : dc.vertercies)
			{
				vertercies.vArray.append({gfx.WorldToScreenPos(v), dc.color});
			}
			
			});
		gfx.Draw(vertercies);
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