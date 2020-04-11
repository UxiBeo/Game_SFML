#pragma once
#include "entt/entt.hpp"
#include <set>
struct ParentComponent
{
	std::set<entt::entity> childEntities;
};