#pragma once
#include "../System/ISystemECS.h"
#include "../HashStringDataBase.h"
#include "../Component/ParentChildrenComponent.h"
class CleanDeathSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final
	{
		auto view = ECS.view<DestroyMe, ParentComponent>();
		ECS.destroy(view.begin(), view.end());

		auto view1 = ECS.view<DestroyMe>();
		ECS.destroy(view1.begin(), view1.end());
	}
};