#pragma once
#include "SFML/Graphics.hpp"
#include "System/IDrawSystem.h"
#include "MapLayer.h"
//moving grid not working yet
class Grid : public IDrawSystem
{
public:
	void LoadFromFile(entt::hashed_string filename);
	virtual void Draw(Graphics& gfx, entt::registry& ECS) const final;
private:
	std::vector<MapLayer> layers;
};
