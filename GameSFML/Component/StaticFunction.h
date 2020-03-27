#pragma once
#include "Box2D/Box2D.h"
#include "entt/entt.hpp"
#include "PhysicComponent.h"
namespace Sfunc
{
	void AddPhysic(entt::entity entity, entt::registry& ECS, const b2BodyDef& Bdef, const b2FixtureDef& fixDef)
	{
		if (auto* engine = ECS.try_ctx<PhysicEngine>(); engine)
		{
			auto& physic = ECS.assign<PhysicComponent>(entity, engine->CreateBody(&Bdef));
			physic->CreateFixture(&fixDef);
			physic->SetUserEntity(entity);
		}
	}
	void DestroyPhysicComponent(entt::registry& ECS, entt::entity entity)
	{
		if (auto* engine = ECS.try_ctx<PhysicEngine>(); engine)
		{
			engine->DestroyBody(ECS.get<PhysicComponent>(entity));
		}
	}
};