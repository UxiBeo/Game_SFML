#pragma once
#include "entt/entt.hpp"
#include "../HashStringDataBase.h"
#include <execution>
class ISystemECS
{
public:
	virtual void Update(entt::registry& ECS) = 0;
};