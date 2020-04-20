#pragma once
#include "../System/ISystemECS.h"
#include "box2d/box2d.h"
class MyQueryCallback : public b2QueryCallback {
public:
    std::vector<entt::entity> foundBodies;

    bool ReportFixture(b2Fixture* fixture) {
        foundBodies.emplace_back(fixture->GetBody()->GetUserEntity());
        return true;//keep going to find all fixtures in the query area
    }
};
class ClickSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final
	{
        /*MyQueryCallback queryCallback;
        auto pos = Locator::Graphic::ref().MouseToWorldPos(Locator::Mouse::ref().GetPos());
       
        b2AABB aabb;
        aabb.lowerBound = pos - b2Vec2(0.1f,0.1f);
        aabb.upperBound = pos + b2Vec2(0.1f, 0.1f);
        m_world->QueryAABB(&queryCallback, aabb);*/
	}
};