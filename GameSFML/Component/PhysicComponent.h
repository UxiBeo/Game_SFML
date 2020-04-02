#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "Box2D/Box2D.h"

namespace Physic
{
	enum Fillter {
		STATIC = 0x0001,
		ENEMY = 0x0002,
		PLAYER = 0x0004,
		BULLET = 0x0006
	};


	using Engine = b2World;
	using Component = b2Body*;
	using SensorWith = std::vector<entt::entity>;


	using SensorIn = std::vector<entt::entity>;
	using SensorOut = std::vector<entt::entity>;
	using ColliedWith = std::vector<entt::entity>;
	struct KeepColliedData{};
	struct KeepSensorData{};
}