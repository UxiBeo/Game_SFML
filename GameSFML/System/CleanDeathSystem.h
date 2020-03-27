#pragma once
#include "../System/ISystemECS.h"
#include "../HashStringDataBase.h"
class CleanDeathSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) final
	{
		auto view = ECS.view<entt::tag<Database::DestroyMe>>();
		ECS.destroy(view.begin(), view.end());
	}
};