#pragma once
#include <memory>
#include <functional>
#include <vector>
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
	using PairEntity = std::pair<entt::entity, entt::entity>;
	void BeginContact(b2Contact* contact) final
	{
		auto* ptrBody1 = contact->GetFixtureA()->GetBody();
		auto* ptrBody2 = contact->GetFixtureB()->GetBody();
		if (ptrBody1->GetType() == b2BodyType::b2_staticBody || ptrBody2->GetType() == b2BodyType::b2_staticBody) return;

		data.push_back({ ptrBody1->GetUserEntity(), ptrBody2->GetUserEntity() });
	}
	void ClearData()
	{
		data.clear();
	}
	const std::vector<PairEntity>& ReadData() const
	{
		return data;
	}
private:
	std::vector<PairEntity> data;
};
struct PhysicEngine
{
	PhysicEngine(b2Vec2 vec = { 0.0f,0.0f })
	{
		Engine = std::make_unique<b2World>(vec);
	}
	std::unique_ptr<b2World> Engine;
};
struct PhysicComponent
{
	PhysicComponent(entt::entity entity, PhysicEngine& physicEngine, const b2BodyDef& bodyDef = b2BodyDef(), const b2FixtureDef& fixtureDef = b2FixtureDef())
	{
		body = { physicEngine.Engine->CreateBody(&bodyDef),[&physicEngine](b2Body* pBody) {physicEngine.Engine->DestroyBody(pBody); } };
		body->CreateFixture(&fixtureDef);
		body->SetUserEntity(entity);
		
	}
	PhysicComponent() = default;
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
};