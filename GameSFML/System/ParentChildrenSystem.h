#pragma once
#include "ISystemECS.h"

class ParentChildrenSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
	static void DestroyParentComponent(entt::registry& ECS, entt::entity entity);
};