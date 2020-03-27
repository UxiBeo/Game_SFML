#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AnimationComponent.h"
#include "../System/WorldTimerSystem.h"
class AnimationSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) final
	{
		auto* worldTime = ECS.try_ctx<WorldTimer>();
		if (worldTime == nullptr) return;
		float dt = worldTime->dt;

		auto view = ECS.view<AnimationComponent>(entt::exclude<AnimNotify>);
		std::for_each(std::execution::par, view.begin(), view.end(), [&view, &ECS, dt](auto entity) {
			auto& animation = view.get<AnimationComponent>(entity);
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
		ECS.view<AnimNotify, AnimationComponent>().each([dt,&ECS](auto entity, const AnimNotify& notify, AnimationComponent& animation) {
			if (animation.isUpdate)
			{
				animation.curFrameTime += dt;
				unsigned char beginFrame = animation.iCurFrame;
				//const auto prevFrame = animation.iCurFrame;
				bool isStep = false;
				while (animation.curFrameTime >= animation.holdTime)
				{
					animation.curFrameTime -= animation.holdTime;
					animation.iCurFrame++; 
					isStep = true;
					if (animation.iCurFrame > animation.endFrame)
					{
						animation.iCurFrame = animation.beginFrame;
					}
				}
				if (!isStep) return;

				for (auto& p : notify)
				{
					if (p.second)
					{
						if (beginFrame < animation.iCurFrame)
						{
							if (beginFrame < p.first && p.first <= animation.iCurFrame)
							{
								p.second(entity, ECS);
							}
						}
						else if(!(beginFrame <= p.first && p.first <= animation.iCurFrame))
						{
							p.second(entity, ECS);
						}
					}
					
				}
				
			}
			});
	}
};