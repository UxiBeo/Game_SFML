#pragma once
#include "entt/entt.hpp"
#include "../Component/ControllerComponent.h"
#include "../Component/StateMachineComponent.h"
#include "../Component/AnimationComponent.h"
#include "../Component/TagRule.h"
struct PlayerAnimationState
{
	static void Idle(entt::registry& ECS, entt::entity entity, FSM::State& st)
	{
		if (ECS.has<MontagePlaying>(entity))
		{
			st.mrD.connect<PlayerAnimationState::Montage>();
			return;
		}
		
		const auto& ct = ECS.ctx<PlayerController>();
		if (ct.direction.x == 0 && ct.direction.y == 0) return;

		Locomotion(ct, ECS.get<AnimationComponent>(entity));
		st.mrD.connect<&PlayerAnimationState::Moving>();
	}
	static void Moving(entt::registry& ECS, entt::entity entity, FSM::State& st)
	{
		if (ECS.has<MontagePlaying>(entity))
		{
			st.mrD.connect<PlayerAnimationState::Montage>();
			return;
		}

		const auto& ct = ECS.ctx<PlayerController>();
		auto& ac = ECS.get<AnimationComponent>(entity);
		if (ct.direction.x == 0 && ct.direction.y == 0)
		{
			SetAnimation(3, ac);
			st.mrD.connect<&PlayerAnimationState::Idle>();
			return;
		}
		Locomotion(ct, ac);
		
	}
	static void Montage(entt::registry& ECS, entt::entity entity, FSM::State& st)
	{
		if (ECS.has<MontagePlaying>(entity)) return;

		const auto& ct = ECS.ctx<PlayerController>();
		auto& ac = ECS.get<AnimationComponent>(entity);

		if (ct.direction.x == 0 && ct.direction.y == 0)
		{
			SetAnimation(3, ac);
			st.mrD.connect<&PlayerAnimationState::Idle>();
			return;
		}
		Locomotion(ct, ac);
		st.mrD.connect<&PlayerAnimationState::Moving>();
	}
private:
	static void SetAnimation(uint8_t setPosition, AnimationComponent& ac)
	{
		uint8_t iSet = ac.iSet % 4 + setPosition * 4;
		const auto& s = ac.ar->sets[iSet];
		ac.iSet = iSet;
		ac.iBegin = s.iBegin;
		ac.iEnd = s.iEnd;
		ac.iCurrent = ac.iBegin;
	}
	static void BlendSpace(uint8_t index, AnimationComponent& ac)
	{
		if (index == ac.iSet) return;
		const auto& s = ac.ar->sets[index];
		const auto oBegin = ac.ar->sets[ac.iSet].iBegin;
		float curInter = float(ac.iCurrent - oBegin) / float(ac.ar->sets[ac.iSet].iEnd - oBegin);
		ac.iSet = index;
		ac.iBegin = s.iBegin;
		ac.iEnd = s.iEnd;
		ac.iCurrent = uint8_t(curInter * float(ac.iEnd - ac.iBegin)) + ac.iBegin;
	}
	static void Locomotion(const PlayerController& ct, AnimationComponent& ac)
	{
		if (ct.direction.x > 0)
		{
			BlendSpace(0u, ac);
			return;
		}

		if (ct.direction.x < 0)
		{
			BlendSpace(2u, ac);
			return;
		}

		if (ct.direction.y < 0)
		{
			BlendSpace(1u, ac);
		}
		else
		{
			BlendSpace(3u, ac);
		}
		
	}
};