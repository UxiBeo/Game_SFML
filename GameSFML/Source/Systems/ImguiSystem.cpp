#include "../../System/ImguiSystem.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "../../Component/TimerComponent.h"
#include "../../Component/Ability/MeleeAttack.h"
#include "../../Component/Ability/ProjectileAttack.h"
void ImguiUpdateSystem::Update(entt::registry& ECS) const
{
	ImGui::SFML::Update(ECS.ctx<Graphics>().getRenderWindow(), ECS.ctx<Timer::World>().time);
	ShowProjectTileWindow(ECS);
}

void ImguiUpdateSystem::ShowMeleeWindow(entt::registry& ECS) const
{
	if (ImGui::Begin("Meelee Attack"))
	{
		float min = -10.0f;
		float max = 10.0f;
		ECS.view<MeleeAttack>().each([&min, &max](auto entity, MeleeAttack& ma) {
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

void ImguiUpdateSystem::ShowProjectTileWindow(entt::registry& ECS) const
{
	if (ImGui::Begin("Projecttile Attack"))
	{
		float min = -10.0f;
		float max = 10.0f;
		ECS.view<ProjectileAttack>().each([&min, &max](auto entity, ProjectileAttack& ma) {
			int anime = (int)ma.projectileAnimation;
			ImGui::SliderInt("P Animation", &anime, 0, 15);
			ImGui::SliderFloat("P Speed", &ma.projectileSpeed, 1.0f, 4.0f, "%.1f");
			ImGui::SliderFloat("attackOffsetD.x", &ma.attackOffsetD.x, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetD.y", &ma.attackOffsetD.y, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetU.x", &ma.attackOffsetU.x, min, max, "%.1f");
			ImGui::SliderFloat("attackOffsetU.y", &ma.attackOffsetU.y, min, max, "%.1f");
			ma.projectileAnimation = (uint8_t)anime;
			});


	}
	ImGui::End();
}

void ImguiDrawSystem::Draw(Graphics& gfx, entt::registry& ECS) const
{
	ImGui::SFML::Render(gfx.getRenderWindow());
}
