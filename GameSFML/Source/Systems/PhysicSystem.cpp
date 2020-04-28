#pragma once
#include "../../System/PhysicSystem.h"
#include "../../System/WorldTimerSystem.h"
#include <future>
#include <execution>
void PhysicSystem::BeginPlay(entt::registry& ECS)const
{
	auto& pe = ECS.set<Physic::Engine>(b2Vec2(0.0f, 0.0f));
	auto& mrListner = ECS.set<Physic::ContactListener>();
	pe.SetContactListener(&mrListner);

	ECS.on_destroy<Physic::Component>().connect<&PhysicSystem::DestroyPhysicComponent>();
}
void PhysicSystem::Update(entt::registry& ECS)const
{
	if (auto* physicEngine = ECS.try_ctx<Physic::Engine>(); physicEngine)
	{
		physicEngine->Step(ECS.ctx<Timer::World>().dt, 4, 2);
		ReactPhysic(ECS);
	}
}

void PhysicSystem::AddPhysic(entt::entity entity, entt::registry& ECS, const b2BodyDef& Bdef, const b2FixtureDef& fixDef)
{
	if (auto* engine = ECS.try_ctx<Physic::Engine>(); engine)
	{
		auto& pc = ECS.assign<Physic::Component>(entity, engine->CreateBody(&Bdef));
		pc->CreateFixture(&fixDef);
		pc->SetUserEntity(entity);
	}
}

void PhysicSystem::ReactPhysic(entt::registry& ECS) const
{
	auto* mrLisner = ECS.try_ctx<Physic::ContactListener>();
	if (mrLisner == nullptr) return;
	
	const auto& data = mrLisner->ReadData();

	for (const auto& i : data[0])
	{
		if (auto* ls = ECS.try_get<Physic::Listener<Physic::Type::Normal>>(i.first); ls)
		{
			if (ls->mrD)
				ls->mrD(ECS, i.first, i.second);
		}
		if (auto* ls = ECS.try_get<Physic::Listener<Physic::Type::Normal>>(i.second); ls)
		{
			if (ls->mrD)
				ls->mrD(ECS, i.second, i.first);
		}
	}
	for (const auto& i : data[1])
	{
		auto& ls = ECS.get<Physic::Listener<Physic::Type::SensorIn>>(i.first);
		if (ls.mrD)
			ls.mrD(ECS, i.first, i.second);
	}
	for (const auto& i : data[2])
	{
		auto& ls = ECS.get<Physic::Listener<Physic::Type::SensorOut>>(i.first);
		if (ls.mrD)
			ls.mrD(ECS, i.first, i.second);
	}
	ECS.view<Physic::Sensor, Physic::Listener<Physic::Type::Sensor>>().each([&ECS](auto entity,
		const Physic::Sensor& ss, Physic::Listener<Physic::Type::Sensor>& ls) {
			if (ls.mrD)
				ls.mrD(ECS, entity, ss);
		});
	mrLisner->ClearAll();
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
