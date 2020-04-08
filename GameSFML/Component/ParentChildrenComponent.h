#pragma once
#include "entt/entt.hpp"
struct ParentComponent
{
	std::vector<entt::entity> childEntities;
};
struct RemoveChildrent
{
	std::vector<entt::entity> entties;
};