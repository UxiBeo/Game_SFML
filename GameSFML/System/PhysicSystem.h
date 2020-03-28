#pragma once
#include "../Component/PhysicComponent.h"
#include "../Component/CollisionRespondComponent.h"
#include "../System/WorldTimerSystem.h"
#include "../System/ISystemECS.h"

class PhysicSystem final : public ISystemECS
{
	void BeginPlay(entt::registry& ECS) final
	{
		ECS.on_destroy<PhysicComponent>().connect<&PhysicSystem::DestroyPhysicComponent>();
	}
	void Update(entt::registry& ECS) final
	{
		auto* worldTime = ECS.try_ctx<WorldTimer>();
		if (worldTime == nullptr) return;
		float dt = worldTime->dt;

		if (auto* physicEngine = ECS.try_ctx<PhysicEngine>(); physicEngine)
		{
			physicEngine->Step(dt, 4, 2);
		}

		if (auto* mrLisner = ECS.try_ctx<Box2DContactListener>(); mrLisner)
		{
			mrLisner->Sort();
			const auto& data = mrLisner->ReadData();

			for (const auto& i : data[0])
			{
				if (ECS.has<Physic::FColliedWith>(i.first))
				{
					ECS.assign_or_replace<Physic::ColliedWith>(i.first).emplace_back(i.second);
				}
				if (ECS.has<Physic::FColliedWith>(i.second))
				{
					ECS.assign_or_replace<Physic::ColliedWith>(i.second).emplace_back(i.first);
				}
			}
			for (const auto& i : data[1])
			{
				ECS.assign_or_replace<Physic::SensorIn>(i.first).data.emplace_back(i.second);
			}
			for (const auto& i : data[2])
			{
				ECS.assign_or_replace<Physic::SensorOut>(i.first).data.emplace_back(i.second);
			}
			mrLisner->ClearAll();
		}
	}
private:
	static void DestroyPhysicComponent(entt::registry& ECS, entt::entity entity)
	{
		if (auto* engine = ECS.try_ctx<PhysicEngine>(); engine)
		{
			engine->DestroyBody(ECS.get<PhysicComponent>(entity));
		}
	}
};