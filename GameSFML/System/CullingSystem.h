#pragma once
#include "../System/ISystemECS.h"
#include "entt/entt.hpp"
#include "box2d/box2d.h"
struct CullingQuerySelector final : public b2QueryCallback
{
	bool ReportFixture(b2Fixture* fixture) final;
	void Sort();
	std::vector<entt::entity> foundBodies;
};
class CullingSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) const final;
	void BeginPlay(entt::registry& ECS) const final;
};