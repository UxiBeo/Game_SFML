#pragma once
#include "../System/ISystemECS.h"
#include "entt/entt.hpp"
#include "box2d/box2d.h"
class CullingSystem final : public ISystemECS
{
	struct CullingQuerySelector final : public b2QueryCallback
	{
		bool ReportFixture(b2Fixture* fixture) final;
		void Sort();
		std::vector<entt::entity> foundBodies;
	};
public:
	void Update(entt::registry& ECS) final;
private:
	CullingQuerySelector cullingQueryCallback;
};