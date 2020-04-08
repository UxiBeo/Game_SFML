#pragma once

#include "../../Component/AnimationComponent.h"
#include "../../System/WorldTimerSystem.h"
#include "../../System/AnimationSystem.h"

void AnimationSystem::Update(entt::registry& ECS)
{
	float dt = ECS.ctx<Timer::World>().dt;

	auto group = ECS.view<AnimationComponent>();
	std::for_each(std::execution::par, group.begin(), group.end(), [&group, dt](auto entity) {
		auto& animation = group.get<AnimationComponent>(entity);
		if (animation.isUpdate)
		{
			animation.curFrameTime += dt;
			//const auto prevFrame = animation.iCurFrame;
			while (animation.curFrameTime >= animation.holdTime)
			{
				animation.curFrameTime -= animation.holdTime;
				animation.iCurFrame++;
				if (animation.iCurFrame > animation.endFrame)
				{
					animation.iCurFrame = animation.beginFrame;
				}
			}
		}
		});
	
	ECS.group<AnimationComponent, AnimNotify>().each([&group, &ECS](auto entity, const auto& ani, const auto& noti) {
		
		if (noti.notifyDelegate && noti.triggerTime >= ani.curFrameTime)
		{
			noti.notifyDelegate(entity, ECS);
		}
		});
	
}
