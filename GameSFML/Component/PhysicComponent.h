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
	enum class Type
	{
		Normal,
		Sensor,
		SensorIn,
		SensorOut
	};
	struct Sensor
	{
		std::vector<entt::entity> entities;
	};
	using Engine = b2World;
	using Component = b2Body*;
	template <Type T>
	struct Listener
	{
		entt::delegate<void(entt::registry&, const entt::entity, const entt::entity)> mrD;
	};
	template<>
	struct Listener<Type::Sensor>
	{
		entt::delegate<void(entt::registry&, const entt::entity, const Sensor&)> mrD;
	};
}