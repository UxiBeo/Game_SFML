#pragma once
#include "../System/ISystemECS.h"
#include "../Component/PhysicComponent.h"
#include "../Component/ControllerComponent.h"
#include "../Component/TimerComponent.h"
class MoveCameraSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final
	{
		auto& gfx = ECS.ctx<Graphics>();
		auto& controller = ECS.ctx<PlayerController>();
		auto& gp = ECS.get<TargetPosition>(controller.entity);
		gp.goalPos = gfx.MouseToWorldPos(controller.mousePos);
		gp.mousePos = controller.mousePos;
		gp.curPos = ECS.get<Physic::Component>(controller.entity)->GetPosition();
		gfx.MoveViewport(gp.curPos, gp.mousePos);
	}
private:
};
/* Move Camera by mouse
class MoveCameraSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto& mouse = Locator::Mouse::ref();
		while (!mouse.IsEmpty())
		{
			const auto mouseEvent = mouse.Read();
			switch (mouseEvent.GetType())
			{
			case Mouse::Event::Type::RPress:
				engaged = true;
				lastPos = mouse.GetPos();
				break;
			case Mouse::Event::Type::RRelease:
				engaged = false;
				break;
			}

		}
		
		if (engaged)
		{
			const auto curPos = mouse.GetPos();
			Locator::Graphic::ref().MoveViewport(lastPos - mouse.GetPos());
			lastPos = curPos;
		}
	}
private:
	bool engaged = false;
	sf::Vector2i lastPos;
};*/

