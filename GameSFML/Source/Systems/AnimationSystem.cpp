#pragma once

#include "../../Component/AnimationComponent.h"
#include "../../System/WorldTimerSystem.h"
#include "../../System/AnimationSystem.h"

void AnimationSystem::Update(entt::registry& ECS) const
{
	float dt = ECS.ctx<Timer::World>().dt;
	UpdateAniamtionMontage(ECS, dt);
	
	auto view = ECS.view<AnimationComponent>(entt::exclude<MontagePlaying>);
	std::for_each(std::execution::par, view.begin(), view.end(), [&view, dt](auto entity) {
		auto& ac = view.get<AnimationComponent>(entity);
		ac.curTime += dt;
		while (ac.curTime >= ac.frameTime)
		{
			ac.curTime -= ac.frameTime;
			ac.iCurrent++;
			if (ac.iCurrent > ac.iEnd)
			{
				ac.iCurrent = ac.iBegin;
			}
		}
		});
}

void AnimationSystem::UpdateAnimationNotify(entt::registry& ECS, const float dt) const
{
	ECS.view<AnimNotify>().each([&ECS, &dt](auto entity, AnimNotify& an) {
		if (an.triggerD)
		{
			an.curTime += dt;
			if (an.curTime >= an.triggerTime)
			{
				an.triggerD(entity, an.listenerE, ECS);
				ECS.remove<AnimNotify>(entity);
			}
			return;
		}

		assert(false);
		});
}

void AnimationSystem::UpdateAniamtionMontage(entt::registry& ECS, const float dt) const
{
	ECS.view<MontagePlaying, AnimationComponent>().each([&dt, &ECS](auto entity, auto, AnimationComponent& ac) {
		ac.curTime += dt;
		while (ac.curTime >= ac.frameTime)
		{
			ac.curTime -= ac.frameTime;
			ac.iCurrent++;
			if (ac.iCurrent > ac.iEnd)
			{
				ac.curTime = 0.0f;
				ac.iCurrent = ac.iBegin;
				ECS.remove<MontagePlaying>(entity);
				break;
			}
		}
		});
	UpdateAnimationNotify(ECS, dt);

	ECS.view<PlayAnimationMontage, AnimationComponent>().each([&ECS](auto entity, const PlayAnimationMontage& am, AnimationComponent& ac) {
		ac.curTime = 0.0f;
		ac.iSet = am.iSet;
		ac.iBegin = ac.ar->sets[am.iSet].iBegin;
		ac.iCurrent = ac.iBegin;
		ac.iEnd = ac.ar->sets[am.iSet].iEnd;
		ECS.assign_or_replace<MontagePlaying>(entity);

		if (ECS.has<AnimNotify>(entity))
		{
			auto& an = ECS.get<AnimNotify>(entity);
			if (an.interrupD)
			{
				an.interrupD(entity, an.listenerE, ECS);
			}
			if (am.an.triggerD || am.an.interrupD)
			{
				an = am.an;
			}
		}
		else if(am.an.triggerD || am.an.interrupD)
		{
			ECS.assign<AnimNotify>(entity) = am.an;
		}
		
		
		});
	ECS.clear<PlayAnimationMontage>();
}
