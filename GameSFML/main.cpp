#include "entt/entt.hpp"
#include "MainWindow.h"
#include "World.h"
int main()
{
	entt::registry ECS;
	auto& wnd = ECS.set<MainWindow>();
	auto& mouse = ECS.set<Mouse>();
	auto& kbd = ECS.set<Keyboard>();
	auto& gfx = ECS.set<Graphics>();
	auto& world = ECS.set<World>(ECS);

	while (wnd.Update(mouse, kbd, gfx.getRenderWindow()))
	{
		gfx.BeginFrame();
		world.Update(ECS);
		world.Draw(gfx, ECS);
		gfx.EndFrame();
	}

	return 0;
}