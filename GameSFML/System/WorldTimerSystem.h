#pragma once
#include "../System/ISystemECS.h"
#include "../Component/TimerComponent.h"
#include <optional>
class WorldTimerSystem final : public ISystemECS
{
public:
	void BeginPlay(entt::registry& ECS) final;
	void Update(entt::registry& ECS) final;
	
private:
	static void DestroyCurrentActiveTimer(entt::registry& ECS, entt::entity entity)
	{
		auto& con = ECS.get<CurrentActiveTimer>(entity);
		ECS.destroy(con.begin(), con.end());
	}
	const static std::optional<size_t> binary_find(const CurrentActiveTimer& vec, entt::entity entity)
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), entity,
			[](const entt::entity lhs, const entt::entity entity) { return lhs < entity; });
		return it != vec.end() && *it == entity ? std::distance(vec.begin(), it) : std::optional<size_t>{};
	}
};