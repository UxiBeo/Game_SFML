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
	struct Sensor
	{
		std::vector<entt::entity> currentE;
		std::vector<entt::entity> inE;
		std::vector<entt::entity> outE;
		entt::delegate<void(entt::registry&, Sensor&)> mrDelegate;
	};
	using Engine = b2World;
	using Component = b2Body*;

	struct Collied
	{
		std::vector<entt::entity> otherEntities;
		entt::delegate<void(entt::registry&, Collied&)> mrDelegate;
	};
	struct KeepColliedData{};
	struct ContexObserver
	{
		entt::observer collied;
		entt::observer sensor;
	};
}