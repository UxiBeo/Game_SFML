#pragma once
#include "../Component/TimerComponent.h"

class WorldTimerSystem final : public ISystemECS
{
public:
	void BeginPlay(entt::registry& ECS) final
	{
		ECS.on_destroy<CurrentActiveTimer>().connect<&WorldTimerSystem::DestroyCurrentActiveTimer>();
	}
	void Update(entt::registry& ECS) final
	{
		auto* worldTime = ECS.try_ctx<WorldTimer>();
		if (worldTime == nullptr) return;
		worldTime->dt = std::min(float(worldTime->clock.restart().asMilliseconds()) / 1000.0f, worldTime->maxdt);
		worldTime->worldTimer += worldTime->dt;
		float dt = worldTime->dt;
		
		ECS.view<OneTimeTimer>().each([dt, &ECS](auto entity, OneTimeTimer& timer) {
			if (!timer.delegate)
			timer.curTime += dt;
			if (timer.curTime >= timer.maxTime)
			{
				if (timer.delegate)
					timer.delegate(entity, ECS);

				ECS.assign<SelfDeleteTimer>(entity, timer.owner);
			}
				
			});

		ECS.view<LoopTimer>().each([dt, &ECS](auto entity, LoopTimer& timer) {
			if (!timer.delegate) return;

			timer.curTime += dt;
			while (timer.curTime >= timer.intervalTime)
			{
				timer.curTime -= timer.intervalTime;
				if (timer.delegate)
					timer.delegate(entity, ECS);
				if (!timer.isInfinity)
				{
					timer.curLoop++;
					if (timer.curLoop >= timer.intervalTime)
					{
						ECS.assign<SelfDeleteTimer>(entity, timer.owner);
					}
				}
			}
				
			});
		auto view = ECS.view<SelfDeleteTimer>();
		view.each([&ECS](auto entity, SelfDeleteTimer& timer) {

			auto* con = ECS.try_get<CurrentActiveTimer>(timer.owner);
			if (con)
			{
				std::sort(std::execution::par_unseq, con->begin(), con->end());
				if (auto it = binary_find(*con, entity))
				{
					std::swap((*con)[*it], con->back());
					con->pop_back();
				}
				else
					assert(false && "Can't find timer");
			}
			});
		ECS.destroy(view.begin(), view.end());
	}
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