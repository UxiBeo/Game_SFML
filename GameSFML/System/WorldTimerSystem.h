#pragma once
#include "../System/ISystemECS.h"
#include "SFML/System/Clock.hpp"
struct WorldTimer
{
	float dt = 0.0f;
	float worldTimer = 0.0f;
	float maxdt = 0.5f;
	sf::Clock clock;
};
class WorldTimerSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) final
	{
		if (auto* worldTime = ECS.try_ctx<WorldTimer>(); worldTime)
		{
			worldTime->dt = std::min(float(worldTime->clock.restart().asMilliseconds()) / 1000.0f, worldTime->maxdt);
			worldTime->worldTimer += worldTime->dt;
		}
	}
};