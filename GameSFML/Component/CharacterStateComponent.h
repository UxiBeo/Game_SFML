#pragma once
#include "entt/entt.hpp"
#include "../Component/ControllerComponent.h"
#include "../Component/StateMachineComponent.h"
struct PlayerAnimationState
{
	static void Idle(entt::registry& ECS, entt::entity entity, FSM::State& st)
	{
		const auto& ct = ECS.ctx<PlayerController>();
		if (ct.direction.x == 0 && ct.direction.y == 0) return;
		Locomotion(ct, ECS.get<AnimationComponent>(entity));
		st.mrD.connect<&PlayerAnimationState::Moving>();
	}
	static void Moving(entt::registry& ECS, entt::entity entity, FSM::State& st)
	{
		const auto& ct = ECS.ctx<PlayerController>();
		auto& ac = ECS.get<AnimationComponent>(entity);
		if (ct.direction.x == 0 && ct.direction.y == 0)
		{
			ac.iEnd = ac.iCurrent;
			ac.iBegin = ac.iCurrent;
			st.mrD.connect<&PlayerAnimationState::Idle>();
			return;
		}
		Locomotion(ct, ac);
		
	}
private:
	static void BlendSpace(uint8_t index, AnimationComponent& ac)
	{
		if (index == ac.iSet && ac.iBegin != ac.iEnd) return;
		const auto& s = ac.ar->sets[index];
		const auto oBegin = ac.ar->sets[ac.iSet].iBegin;
		const float curInter = float(ac.iCurrent - oBegin) / float(ac.ar->sets[ac.iSet].iEnd - oBegin);
		ac.iSet = index;
		ac.iBegin = s.iBegin;
		ac.iEnd = s.iEnd;
		ac.frameTime = s.frameTime;
		ac.iCurrent = uint8_t(curInter * float(ac.iEnd - ac.iBegin)) + ac.iBegin;
	}
	static void Locomotion(const PlayerController& ct, AnimationComponent& ac)
	{
		if (ct.direction.x != 0 && ct.direction.y != 0)
		{
			if (ct.direction.x > 0)
			{
				if (ct.direction.y < 0)
				{
					BlendSpace(1u, ac);
					return;
				}
				BlendSpace(7u, ac);
				return;
			}

			if (ct.direction.y < 0)
			{
				BlendSpace(3u, ac);
				return;
			}
			BlendSpace(5u, ac);
			return;
		}
		
		if (ct.direction.y == 0)
		{
			if (ct.direction.x > 0)
			{
				BlendSpace(0u, ac);
				return;
			}
			BlendSpace(4u, ac);
			return;
		}
		
		if (ct.direction.y > 0)
		{
			BlendSpace(6u, ac);
		}
		else
		{
			BlendSpace(2u, ac);
		}
		
	}
};