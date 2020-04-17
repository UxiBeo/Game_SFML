#pragma once
#include "../../System/PhysicSystem.h"
#include "../../System/WorldTimerSystem.h"
#include <future>
#include <execution>
void PhysicSystem::BeginPlay(entt::registry& ECS)
{
	auto& mrListner = ECS.set<Physic::ContactListener>();
	ECS.ctx<Physic::Engine>().SetContactListener(&mrListner);

	auto& obs = ECS.set<Physic::ContexObserver>();
	obs.collied.connect(ECS, entt::collector.replace<Physic::Collied>());
	obs.sensor.connect(ECS, entt::collector.replace<Physic::Sensor>());

	ECS.on_destroy<Physic::Component>().connect<&PhysicSystem::DestroyPhysicComponent>();
}
void PhysicSystem::Update(entt::registry& ECS)
{
	if (auto* physicEngine = ECS.try_ctx<Physic::Engine>(); physicEngine)
	{
		physicEngine->Step(ECS.ctx<Timer::World>().dt, 4, 2);
		ProcessContactData(ECS);
		ReactPhysic(ECS);
	}
}

void PhysicSystem::AddPhysic(entt::entity entity, entt::registry& ECS, const b2BodyDef& Bdef, const b2FixtureDef& fixDef)
{
	if (auto* engine = ECS.try_ctx<Physic::Engine>(); engine)
	{
		auto& physic = ECS.assign<Physic::Component>(entity, engine->CreateBody(&Bdef));
		physic->CreateFixture(&fixDef);
		physic->SetUserEntity(entity);
	}
}

void PhysicSystem::ReactPhysic(entt::registry& ECS) const
{
	auto& obs = ECS.ctx<Physic::ContexObserver>();
	for (auto e : obs.collied)
	{
		auto& data = ECS.get<Physic::Collied>(e);
		if (data.mrDelegate)
		{
			data.mrDelegate(ECS, data);
		}
	}
	for (auto e : obs.sensor)
	{
		auto& data = ECS.get<Physic::Sensor>(e);
		if (data.mrDelegate)
		{
			data.mrDelegate(ECS, data);
		}
	}
}

void PhysicSystem::ProcessContactData(entt::registry& ECS) const
{
	if (auto* mrLisner = ECS.try_ctx<Physic::ContactListener>(); mrLisner)
	{
		const auto& data = mrLisner->ReadData();

		for (const auto& i : data[0])
		{
			if (ECS.has<Physic::KeepColliedData>(i.first))
				ECS.assign_or_replace<Physic::Collied>(i.first).otherEntities.emplace_back(i.second);

			if (ECS.has<Physic::KeepColliedData>(i.second))
				ECS.assign_or_replace<Physic::Collied>(i.second).otherEntities.emplace_back(i.first);
				
		};

		for (const auto& i : data[1])
		{
			ECS.assign_or_replace<Physic::Sensor>(i.first).inE.emplace_back(i.second);
		};
		for (const auto& i : data[2])
		{
			ECS.assign_or_replace<Physic::Sensor>(i.first).outE.emplace_back(i.second);
		};

		mrLisner->ClearAll();
	}
}

void PhysicSystem::DestroyPhysicComponent(entt::registry& ECS, entt::entity entity)
{
	if (auto* engine = ECS.try_ctx<Physic::Engine>(); engine)
	{
		engine->DestroyBody(ECS.get<Physic::Component>(entity));
	}
}

void Physic::ContactListener::BeginContact(b2Contact* contact)
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

void Physic::ContactListener::EndContact(b2Contact* contact)
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

		data[2].push_back({ ptrBody2->GetUserEntity(), ptrBody1->GetUserEntity() });
	}

}

void Physic::ContactListener::ClearAll()
{
	std::for_each(data.begin(), data.end(), [](std::vector<PairEntity>& con) { con.clear(); });
}
