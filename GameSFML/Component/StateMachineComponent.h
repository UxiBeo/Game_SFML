#pragma once
#include "entt/entt.hpp"
namespace FSM
{
	struct State
	{
		entt::delegate<void(entt::registry&, entt::entity, State&)> mrD;
	};
}