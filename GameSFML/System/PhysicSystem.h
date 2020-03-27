#pragma once
#include "../Component/PhysicComponent.h"
#include "../Component/CollisionRespondComponent.h"
#include "../System/WorldTimerSystem.h"
#include "../System/ISystemECS.h"

class PhysicSystem final : public ISystemECS
{
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
				if (ECS.has<FColliedWithPar>(i.first) || ECS.has<FColliedWithSeg>(i.first))
				{
					ECS.assign_or_replace<ColliedWith>(i.first).emplace_back(i.second);
				}
				if (ECS.has<FColliedWithPar>(i.second) || ECS.has<FColliedWithSeg>(i.second))
				{
					ECS.assign_or_replace<ColliedWith>(i.second).emplace_back(i.first);
				}
			}
			for (const auto& i : data[1])
			{
				ECS.assign_or_replace<SensorIn>(i.first).data.emplace_back(i.second);
			}
			for (const auto& i : data[2])
			{
				ECS.assign_or_replace<SensorOut>(i.first).data.emplace_back(i.second);
			}
			mrLisner->ClearAll();
		}
	}

};