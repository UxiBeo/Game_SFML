#pragma once
#include "../System/ISystemECS.h"
#include "../HashStringDataBase.h"
class CleanDeathSystem : public ISystemECS
{
public:
	void Update(entt::registry& ECS, float dt)
	{
		auto view = ECS.view<entt::tag<Database::DestroyMe>>();
		ECS.destroy(view.begin(), view.end());
	}
};