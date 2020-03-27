#pragma once
#include "../Component/PhysicComponent.h"
#include "entt/entt.hpp"
struct SensorWith
{
	std::vector<entt::entity> data;
};
struct FSensorPar
{
	entt::delegate<void(entt::entity, const std::vector<entt::entity>&, entt::registry&)> func;
};
struct FSensorSeg
{
	entt::delegate<void(entt::entity, const std::vector<entt::entity>&, entt::registry&)> func;
};
struct SensorIn
{
	std::vector<entt::entity> data;
};
struct FSensorIn
{
	entt::delegate<void(entt::entity, const std::vector<entt::entity>&, entt::registry&)> func;
};
struct SensorOut
{
	std::vector<entt::entity> data;
};
struct FSensorOut
{
	entt::delegate<void(entt::entity, const std::vector<entt::entity>&, entt::registry&)> func;
};

using ColliedWith = std::vector<entt::entity>;
using FColliedWithPar = entt::delegate<void(entt::entity, const ColliedWith&, entt::registry&)>;
using FColliedWithSeg = entt::delegate<void(entt::entity, const ColliedWith&, entt::registry&)>;


