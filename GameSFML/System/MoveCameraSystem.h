#pragma once
#include "../System/ISystemECS.h"
#include "../Component/PhysicComponent.h"
#include "../Component/PlayerControllerComponent.h"
#include "../Component/TimerComponent.h"
class MoveCameraSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) final
	{
		auto& gfx = ECS.ctx<Graphics>();
		auto& controller = ECS.ctx<PlayerControllerComponent>();
		controller.mouseWorldPos = gfx.MouseToWorldPos(controller.mouseScreenPos);
		ECS.view<entt::tag<"CameraTracking"_hs>, Physic::Component>().each([&](auto entity, auto, Physic::Component& physic) {
			
			gfx.MoveViewport(physic->GetPosition(), controller.mouseScreenPos);
		});
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

