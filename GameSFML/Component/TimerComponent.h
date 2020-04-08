#pragma once
#include "../System/ISystemECS.h"
#include "SFML/System/Clock.hpp"
namespace Timer
{
	struct World
	{
		float dt = 0.0f;
		float worldTimer = 0.0f;
		float maxdt = 0.5f;
		sf::Clock clock;
	};
	struct OneTime
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
		entt::entity owner;
		entt::delegate<void(entt::entity, entt::registry&)> delegate;
	};

	struct Loop
	{
		float curTime = 0.0f;
		float intervalTime = 0.0f;
		bool isInfinity = false;
		unsigned int curLoop = 0;
		unsigned int maxLoop = 0;
		entt::entity owner;
		entt::delegate<void(entt::entity, entt::registry&)> delegate;
	};
}
