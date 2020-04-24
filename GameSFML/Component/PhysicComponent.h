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
	struct Query : public b2QueryCallback
	{
		const std::vector<entt::entity>& GetEntities(Engine& pe, const b2AABB& aabb)
		{
			pe.QueryAABB(this, aabb);
			return foundBodies;
		}
		bool ReportFixture(b2Fixture* fixture) final
		{
			foundBodies.emplace_back(fixture->GetBody()->GetUserEntity());
			return true;//keep going to find all fixtures in the query area
		}
	private:
		std::vector<entt::entity> foundBodies;
	};
}