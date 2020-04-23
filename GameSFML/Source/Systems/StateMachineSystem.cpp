#pragma once
#include "../../System/StateMachineSystem.h"
#include "../../Component/StateMachineComponent.h"

void StateMachineSystem::Update(entt::registry& ECS)const
{
	auto view = ECS.view<FSM::State>();
	std::for_each(view.begin(), view.end(), [&view, &ECS](auto entity) {
		FSM::State& st = view.get<FSM::State>(entity);
		if (st.mrD)
			st.mrD(ECS, entity, st);
		});
}
