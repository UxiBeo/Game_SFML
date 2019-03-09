#pragma once
#include "GameplayTags.h"
#include "entt/entt.hpp"
#include "Box2D/Box2D.h"
class CleanDeadSystem
{
public:
	void Update(entt::DefaultRegistry& ECSEngine)
	{
		ECSEngine.destroy<DeathTag>();
	}
};
class SpawnEnemySystem
{
public:
	void Update(entt::DefaultRegistry& ECSEngine, b2World& box2DEngine)
	{
		ECSEngine.view<SpawnEnemyInfo, PhysicComponent, sf::Sprite>().each([&ECSEngine,&box2DEngine](auto entity, 
			SpawnEnemyInfo & info, PhysicComponent& physicCom, sf::Sprite& sprite) {

			auto newEntity = ECSEngine.create();
			ECSEngine.assign<HealthComponent>(newEntity, 50.0f);
			ECSEngine.assign<sf::Sprite>(newEntity, sprite);

			{
				b2BodyDef bodyDef;
				b2FixtureDef fixtureDef;
				bodyDef.type = physicCom.body->GetType();
				bodyDef.position = physicCom.body->GetPosition();
				fixtureDef.shape = physicCom.body->GetFixtureList()->GetShape();
				fixtureDef.filter = physicCom.body->GetFixtureList()->GetFilterData();
				fixtureDef.density = physicCom.body->GetFixtureList()->GetDensity();
				fixtureDef.friction = physicCom.body->GetFixtureList()->GetFriction();
				fixtureDef.restitution = physicCom.body->GetFixtureList()->GetRestitution();
				ECSEngine.assign<PhysicComponent>(newEntity, newEntity, box2DEngine, bodyDef, fixtureDef);
			}
		});
		ECSEngine.remove<SpawnEnemyInfo>();
		auto newview = ECSEngine.view<SpawnEnemyInfo>();
	}
};