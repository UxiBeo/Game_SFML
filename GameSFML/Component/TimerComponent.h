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
struct OneTimeTimer
{
	float curTime = 0.0f;
	float maxTime = 0.0f;
	entt::entity owner;
	entt::delegate<void(entt::entity, entt::registry&)> delegate;
};

struct LoopTimer
{
	float curTime = 0.0f;
	float intervalTime = 0.0f;
	bool isInfinity = false;
	unsigned int curLoop = 0;
	unsigned int maxLoop = 0;
	entt::entity owner;
	entt::delegate<void(entt::entity, entt::registry&)> delegate;
};
struct SelfDeleteTimer 
{
	entt::entity owner;
};
struct CancleTimer{};
struct CurrentActiveTimer
{
	void AddEntity(entt::entity entity)
	{
		if (curIndex < maxSize - 1)
		{
			otherEntities[curIndex] = entity;
			curIndex++;
		}
	}
	void RemoveEntity(entt::entity entity)
	{
		if (auto it = std::find(std::execution::par_unseq, otherEntities.begin(), otherEntities.begin() + curIndex, entity); it != otherEntities.end())
		{
			*it = otherEntities[curIndex];

			if (curIndex > 0)
			{
				curIndex--;
			}
		}
		else
			assert(false && "Can't find timer");
	}
	void OnDelete(entt::registry& ECS)
	{
		for (unsigned char i = 0; i < curIndex; i++)
		{
			ECS.destroy(otherEntities[i]);
		}
	}
private:
	constexpr static unsigned char maxSize = 4;
	std::array<entt::entity, maxSize> otherEntities;
	unsigned char curIndex = 0;
};