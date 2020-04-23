#pragma once

#include "../../Component/AnimationComponent.h"
#include "../../System/WorldTimerSystem.h"
#include "../../System/AnimationSystem.h"

void AnimationSystem::Update(entt::registry& ECS) const
{
	float dt = ECS.ctx<Timer::World>().dt;

	auto view = ECS.view<AnimationComponent>();
	std::for_each(std::execution::par, view.begin(), view.end(), [&view, dt](auto entity) {
		auto& ac = view.get<AnimationComponent>(entity);
		if (ac.isUpdate)
		{
			ac.curTime += dt;
			//const auto prevFrame = animation.iCurFrame;
			while (ac.curTime >= ac.frameTime)
			{
				ac.curTime -= ac.frameTime;
				ac.iCurrent++;
				if (ac.iCurrent > ac.iEnd)
				{
					ac.iCurrent = ac.iBegin;
				}
			}
		}
		});
	
	ECS.group<AnimationComponent, AnimNotify>().each([&ECS](auto entity, const auto& ani, const auto& noti) {
		
		if (noti.notifyDelegate && noti.triggerTime >= ani.curTime)
		{
			noti.notifyDelegate(entity, ECS);
		}
		});
	
}
