#pragma once
#include "../Graphics.h"
#include "entt/entt.hpp"
class IDrawSystem
{
public:
	virtual void Draw(Graphics& gfx, entt::registry& ECS) const = 0;
};