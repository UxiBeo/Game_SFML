#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../Component/PhysicComponent.h"
#include "../Component/AnimationComponent.h"
#include "../Component/GameplayTags.h"
#include "../HashStringDataBase.h"
#include "entt/entt.hpp"
#include <algorithm>
#include <execution>
#include <iostream>
class CullingSystem : public ISystemECS
{
	struct CullingQuerySelector final : public b2QueryCallback
	{
		bool ReportFixture(b2Fixture* fixture) final
		{
			foundBodies.emplace_back(fixture->GetBody()->GetUserEntity());
			return true;//keep going to find all fixtures in the query area
		}
		void Sort()
		{
			std::sort(std::execution::par_unseq, foundBodies.begin(), foundBodies.end());
		}
		std::vector<entt::entity> foundBodies;
	};
public:
	void Update(entt::registry& ECS, float dt) final
	{
		if (auto* physicEngine = ECS.try_ctx<PhysicEngine>(); physicEngine)
		{
			ECS.clear<Viewable>();
			cullingQueryCallback.foundBodies.clear();
			auto viewport = Locator::Graphic::ref().GetViewport();
			b2AABB aabb;
			if (viewport.first.x > viewport.second.x)
			{
				std::swap(viewport.first.x, viewport.second.x);
			}
			if (viewport.first.y > viewport.second.y)
			{
				std::swap(viewport.first.y, viewport.second.y);
			}
			aabb.lowerBound = viewport.first;
			aabb.upperBound = viewport.second;
			physicEngine->Engine->QueryAABB(&cullingQueryCallback, aabb);
			for (auto e : cullingQueryCallback.foundBodies)
			{
				ECS.assign<entt::tag<Database::Viewable>>(e);
			}
		}

		
	}
private:
	CullingQuerySelector cullingQueryCallback;
};