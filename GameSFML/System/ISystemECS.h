#pragma once
#include "entt/entt.hpp"
#include "../HashStringDataBase.h"
#include <execution>
struct DestroyMe{};
class ISystemECS
{
public:
	virtual void Update(entt::registry& ECS) = 0;
	virtual void BeginPlay(entt::registry& ECS) {};
};