#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../System/IDrawSystem.h"
class GridUpdateSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS, float dt) final
	{
		if (auto* grid = ECS.try_ctx<Grid>(); grid)
		{
			auto viewport = Locator::Graphic::ref().GetViewportScreen();
			//grid->Culling(viewport.first, viewport.second);
		}
	}
};
class RenderGridSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::registry& ECS) const final
	{
		if (auto* grid = ECS.try_ctx<Grid>(); grid)
		{
			grid->Draw(gfx, ECS);
		}
	}
};