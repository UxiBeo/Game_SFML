#include "../../System/ImguiSystem.h"
#include "../../Component/TimerComponent.h"
#include "../../Component/ImguiComponent.h"
#include "imgui-SFML.h"
void ImguiUpdateSystem::Update(entt::registry& ECS) const
{
	ImGui::SFML::Update(ECS.ctx<Graphics>().getRenderWindow(), ECS.ctx<Timer::World>().time);
	ECS.view<ShowImgui, ImguiComponent>().each([&ECS](auto, auto, const ImguiComponent& ic) {
		assert(ic.mrD);
		ic.mrD(ECS);
		});
}

void ImguiDrawSystem::Draw(Graphics& gfx, entt::registry& ECS) const
{
	ImGui::SFML::Render(gfx.getRenderWindow());
}
