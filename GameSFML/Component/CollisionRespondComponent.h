#pragma once
#include "../Component/PhysicComponent.h"
#include "entt/entt.hpp"
namespace Physic
{
	struct SensorWith
	{
		std::vector<entt::entity> data;
	};
	struct FSensor
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
	using FColliedWith = entt::delegate<void(entt::entity, const ColliedWith&, entt::registry&)>;
	struct CanParallel{};
}



