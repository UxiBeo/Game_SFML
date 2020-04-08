#pragma once
#include "../../System/PhysicSystem.h"
#include "../../System/WorldTimerSystem.h"
#include <future>
#include <execution>
void PhysicSystem::BeginPlay(entt::registry& ECS)
{
	auto& mrListner = ECS.set<Physic::ContactListener>();
	ECS.ctx<Physic::Engine>().SetContactListener(&mrListner);
		
	ECS.on_destroy<Physic::Component>().connect<&PhysicSystem::DestroyPhysicComponent>();
	/*Physic::ObserverCTX& obs = ECS.set<Physic::ObserverCTX>();

	std::get<0>(obs).connect(ECS, entt::collector.group<Physic::ColliedWith>());

	std::get<1>(obs).connect(ECS, entt::collector.group<Physic::SensorIn>());

	std::get<2>(obs).connect(ECS, entt::collector.group<Physic::SensorOut>());*/
}
void PhysicSystem::Update(entt::registry& ECS)
{
	if (auto* physicEngine = ECS.try_ctx<Physic::Engine>(); physicEngine)
	{
		ClearContactData(ECS);
		physicEngine->Step(ECS.ctx<Timer::World>().dt, 4, 2);
		ProcessContactData(ECS);
		HandleSensorData(ECS);
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

void PhysicSystem::HandleSensorData(entt::registry& ECS)
{
	ECS.view<Physic::SensorIn>().each([&ECS](const auto entity, const Physic::SensorIn& sensorIn) {

		auto& senWith = ECS.get_or_assign<Physic::SensorWith>(entity).otherEntities;

		auto& data = ECS.get<Physic::SensorIn>(entity).otherEntities;
		std::vector<entt::entity> merge;
		std::merge(senWith.begin(), senWith.end(), sensorIn.otherEntities.begin(), sensorIn.otherEntities.end(), std::back_inserter(merge));

		if ((senWith.size() + sensorIn.otherEntities.size()) != merge.size()) assert(false && "Sensor out have some element mismatch");
		if (merge.size() == 0) return;

		senWith = std::move(merge);
		});

	//senser Out
	ECS.view<Physic::SensorOut>().each([&ECS](const auto entity, const Physic::SensorOut& sensoOut) {

		auto& senWith = ECS.get_or_assign<Physic::SensorWith>(entity).otherEntities;

		std::vector<entt::entity> diff;
		std::set_difference(senWith.begin(), senWith.end(), sensoOut.otherEntities.begin(), sensoOut.otherEntities.end(), std::back_inserter(diff));

		if ((senWith.size() - diff.size()) != sensoOut.otherEntities.size()) assert(false && "Sensor out have some element mismatch");
		if (diff.size() == 0)
		{
			ECS.remove<Physic::SensorWith>(entity);
			return;
		}

		if (diff.size() < senWith.size())
			senWith = std::move(diff);
		});
}

void PhysicSystem::ClearContactData(entt::registry& ECS)
{
	{
		auto view = ECS.view<Physic::ColliedWith>();
		ECS.destroy(view.begin(), view.end());
	}
	{
		auto view = ECS.view<Physic::SensorIn>();
		ECS.destroy(view.begin(), view.end());
	}
	{
		auto view = ECS.view<Physic::SensorOut>();
		ECS.destroy(view.begin(), view.end());
	}
}

void PhysicSystem::ProcessContactData(entt::registry& ECS)
{
	if (auto* mrLisner = ECS.try_ctx<Physic::ContactListener>(); mrLisner)
	{
		mrLisner->Sort();
		const auto& data = mrLisner->ReadData();

		auto a1 = std::async(std::launch::async, [&] {
			for (const auto& i : data[0])
			{
				if (ECS.has<Physic::KeepColliedData>(i.first))
					ECS.get_or_assign<Physic::ColliedWith>(i.first).otherEntities.emplace_back(i.second);

				if (ECS.has<Physic::KeepColliedData>(i.second))
					ECS.get_or_assign<Physic::ColliedWith>(i.second).otherEntities.emplace_back(i.first);
				
			}});

		auto a2 = std::async(std::launch::async, [&] {
			for (const auto& i : data[1])
			{
				ECS.get_or_assign<Physic::SensorIn>(i.first).otherEntities.emplace_back(i.second);
			}});
		auto a3 = std::async(std::launch::async, [&] {
			for (const auto& i : data[2])
			{
				ECS.get_or_assign<Physic::SensorOut>(i.first).otherEntities.emplace_back(i.second);
			}});

		a1.wait();
		a2.wait();
		a3.wait();

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
	std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](std::vector<PairEntity>& con) { con.clear(); });
}

void Physic::ContactListener::Sort()
{
	std::for_each(std::execution::par_unseq, data.begin() + 1, data.end(), [](std::vector<PairEntity>& con) {
		std::sort(std::execution::par_unseq, con.begin(), con.end());
		});
}
