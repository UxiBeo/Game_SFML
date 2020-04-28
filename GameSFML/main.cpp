#include "imgui.h"
#include "imgui-SFML.h"
#include "entt/entt.hpp"
#include "MainWindow.h"
#include "World.h"
#include "imgui-SFML.h"
int main()
{
	entt::registry ECS;
	auto& wnd = ECS.set<MainWindow>();
	auto& mouse = ECS.set<Mouse>();
	auto& kbd = ECS.set<Keyboard>();
	auto& gfx = ECS.set<Graphics>();
	auto& world = ECS.set<World>(ECS);
	ImGui::SFML::Init(gfx.getRenderWindow());
	while (wnd.Update(mouse, kbd, gfx.getRenderWindow()))
	{
		gfx.BeginFrame();
		world.Update(ECS);
		world.Draw(gfx, ECS);
		gfx.EndFrame();
	}
	ImGui::SFML::Shutdown();
	return 0;
}