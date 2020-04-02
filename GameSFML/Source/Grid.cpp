#pragma once
#include "../Gird.h"
#include "../GameResource.h"
void Grid::LoadFromFile(entt::hashed_string filename)
{
	const auto& map = Codex<TileMapResource>::Retrieve(filename).map;


	for (size_t i = 0; i < map.getLayers().size(); i++)
	{
		layers.emplace_back(map, i);
	}

}

void Grid::Draw(Graphics& gfx, entt::registry& ECS) const
{
	for (const auto& l : layers)
	{
		gfx.Draw(l);
	}
}
