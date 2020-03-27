#pragma once
#include <vector>
#include <execution>
#include "Box2D/Box2D.h"
#include "Locator.h"

enum CollisionFillter {
	STATIC = 0x0001,
	ENEMY = 0x0002,
	PLAYER = 0x0004,
	BULLET = 0x0006
};
class Box2DContactListener final : public b2ContactListener
{
public:
	//2 entity collided waith each other
	using PairEntity = std::pair<entt::entity, entt::entity>;

	//1st element, 2: list of entity collied with entity 1st element
	using CollisionList = std::pair<entt::entity, std::vector<entt::entity>>;
	void BeginContact(b2Contact* contact) final
	{
		auto* ptrBody1 = contact->GetFixtureA()->GetBody();
		auto* ptrBody2 = contact->GetFixtureB()->GetBody();
		
		if (ptrBody1->GetType() == b2BodyType::b2_staticBody || ptrBody2->GetType() == b2BodyType::b2_staticBody) return;
		
		
		if (contact->GetFixtureA()->IsSensor())
		{
			data[1].push_back({ ptrBody1->GetUserEntity(), ptrBody2->GetUserEntity() });
			return;
		}
		if (contact->GetFixtureB()->IsSensor())
		{
			data[1].push_back({ ptrBody2->GetUserEntity(), ptrBody1->GetUserEntity() });
			return;
		}
		data[0].push_back({ ptrBody1->GetUserEntity(), ptrBody2->GetUserEntity() });
	}
	void EndContact(b2Contact* contact) final
	{
		bool sensorA = contact->GetFixtureA()->IsSensor();
		bool sensorB = contact->GetFixtureB()->IsSensor();
		if (sensorA ^ sensorB)
		{
			auto* ptrBody1 = contact->GetFixtureA()->GetBody();
			auto* ptrBody2 = contact->GetFixtureB()->GetBody();

			if (ptrBody1->GetType() == b2BodyType::b2_staticBody || ptrBody2->GetType() == b2BodyType::b2_staticBody) return;


			if (sensorA)
			{
				data[2].push_back({ ptrBody1->GetUserEntity(), ptrBody2->GetUserEntity() });
				return;
			}
			if (sensorB)
			{
				data[2].push_back({ ptrBody2->GetUserEntity(), ptrBody1->GetUserEntity() });
				return;
			}
		}
		
	}
	void ClearAll()
	{
		std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](std::vector<PairEntity>& con) { con.clear(); });
	}
	const std::array<std::vector<PairEntity>, 3>& ReadData() const
	{
		return data;
	}
	void Sort()
	{
		std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](std::vector<PairEntity>& con) {
			std::sort(std::execution::par_unseq, con.begin(), con.end());
			});
	}
private:
private:
	//0: for normal Collision, 1: for sensor in, 2: for sensor out
	std::array<std::vector<PairEntity>, 3> data;
};
using PhysicEngine = b2World;
using PhysicComponent = b2Body*;