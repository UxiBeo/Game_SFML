#pragma once
#include <assert.h>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "GameResource.h"
#include "HashStringDataBase.h"
#include "Component/StaticObjectSpawnInfo.h"
#include "Component/GameplayTags.h"

#include "MapLayer.h"
#include "System/IDrawSystem.h"
//moving grid not working yet
class Grid : public IDrawSystem
{
public:
	void LoadFromFile(entt::hashed_string filename)
	{
		const auto& map = Codex<TileMapResource>::Retrieve(filename).map;

		
		for (size_t i = 0; i < map.getLayers().size(); i++)
		{
			layers.emplace_back(map, i);
		}

	}
	virtual void Draw(Graphics& gfx, entt::registry& ECS) const override
	{
		for (const auto& l : layers)
		{
			gfx.Draw(l);
		}
	}
private:
	std::vector<MapLayer> layers;
};
