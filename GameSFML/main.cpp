#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"
#include "entt/entt.hpp"
#include "MainWindow.h"
#include "World.h"
#include "imgui-SFML.h"
class ImguiManager
{
public:
	ImguiManager(sf::RenderWindow& window) {
		ImGui::SFML::Init(window);
	};
	~ImguiManager()
	{
		ImGui::SFML::Shutdown();
	}
};
int main()
{
	entt::registry ECS;
	auto& wnd = ECS.set<MainWindow>();
	auto& mouse = ECS.set<Mouse>();
	auto& kbd = ECS.set<Keyboard>();
	auto& gfx = ECS.set<Graphics>();
	auto& world = ECS.set<World>(ECS);
	ImguiManager imguiMgr = { gfx.getRenderWindow() };
	try
	{
		while (wnd.Update(mouse, kbd, gfx.getRenderWindow()))
		{
			gfx.BeginFrame();
			world.Update(ECS);
			world.Draw(gfx, ECS);
			gfx.EndFrame();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "caught exception - - - " << e.what() << '\n';
		return 1;
	}
	return 0;
}