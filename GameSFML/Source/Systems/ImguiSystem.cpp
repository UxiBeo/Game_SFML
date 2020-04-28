#include "../../System/ImguiSystem.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "../../Component/TimerComponent.h"
#include "../../Component/Ability/MeleeAttack.h"

void ImguiUpdateSystem::Update(entt::registry& ECS) const
{
	ImGui::SFML::Update(ECS.ctx<Graphics>().getRenderWindow(), ECS.ctx<Timer::World>().time);
	auto view = ECS.view<MeleeAttack>();
	if (ImGui::Begin("Meelee Attack"))
	{
		float min = -10.0f;
		float max = 10.0f;
		view.each([&min, &max](auto entity, MeleeAttack& ma) {
			ImGui::SliderFloat("aoeH.X", &ma.aoeH.x, min, max, "%.1f");
			ImGui::SliderFloat("aoeH.y", &ma.aoeH.y, min, max, "%.1f");
			ImGui::SliderFloat("aoeV.y", &ma.aoeV.y, min, max, "%.1f");
			ImGui::SliderFloat("aoeV.x", &ma.aoeV.x, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetD.x", &ma.attackOffsetD.x, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetD.y", &ma.attackOffsetD.y, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetU.x", &ma.attackOffsetU.x, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetU.y", &ma.attackOffsetU.y, min, max, "%.1f");
			});
		
		
	}
	ImGui::End();
}

void ImguiDrawSystem::Draw(Graphics& gfx, entt::registry& ECS) const
{
	ImGui::SFML::Render(gfx.getRenderWindow());
}
