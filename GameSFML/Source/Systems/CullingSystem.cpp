#pragma once
#include "../../System/CullingSystem.h"
#include "../../Component/PhysicComponent.h"
#include "../../HashStringDataBase.h"

#include <algorithm>
#include <execution>
bool CullingQuerySelector::ReportFixture(b2Fixture* fixture)
{
	foundBodies.emplace_back(fixture->GetBody()->GetUserEntity());
	return true;//keep going to find all fixtures in the query area
}

void CullingQuerySelector::Sort()
{
	std::sort(std::execution::par_unseq, foundBodies.begin(), foundBodies.end());
}

void CullingSystem::Update(entt::registry& ECS) const
{
	if (auto* physicEngine = ECS.try_ctx<Physic::Engine>(); physicEngine)
	{
		ECS.ctx<CullingQuerySelector>();
		ECS.clear<entt::tag<"Viewable"_hs>>();
		/*auto viewport = Locator::Graphic::ref().GetViewport();
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
		physicEngine->QueryAABB(&cullingQueryCallback, aabb);
		for (auto e : cullingQueryCallback.foundBodies)
		{
			ECS.assign<entt::tag<Database::Viewable>>(e);
		}*/
	}


}

void CullingSystem::BeginPlay(entt::registry& ECS) const
{
	ECS.set<CullingQuerySelector>();
}
