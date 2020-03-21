#pragma once
#include "entt/entt.hpp"
#include "../HashStringDataBase.h"
#include "../Component/PlayerControllerComponent.h"
#include "../Component/CharacterStateComponent.h"
#include "../Component/AnimationComponent.h"
#include "../System/ISystemECS.h"
#include "../Component/TagRule.h"
class StateMachineSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS, float dt) final
	{
		if (auto* rule = ECS.try_ctx<TagRule>(); rule)
		{
			ECS.view<CharacterStateComponent, GameplayTag>().each([&rule ,&ECS](auto entity,
				CharacterStateComponent& pStateCom, GameplayTag& tags) {
					if (tags.HasAny(rule->blockMoveTags))
					{
						pStateCom.canMove = false;
					}


				});
		}
		
		//update player control state
		ECS.view<CharacterStateComponent, PlayerControllerComponent>().each([&ECS](auto entity, 
			CharacterStateComponent& pStateCom, PlayerControllerComponent& controller) {
				pStateCom.prevState = pStateCom.curState;
				pStateCom.prevDir =  pStateCom.curDir;
				pStateCom.curDir = controller.direction;
				if (pStateCom.curState != CharacterStateComponent::State::Dead)
				{
					if (controller.shootIsPressed)
					{
						pStateCom.curState = CharacterStateComponent::State::Shooting;
					}
					else
					{
						pStateCom.curState = CharacterStateComponent::State::IdleAndWalking;
					}
				}
			});

		//update Animation State
		ECS.view<CharacterStateComponent, AnimationComponent>().each([&ECS](auto entity,
			CharacterStateComponent& pStateCom, AnimationComponent& anim) {
				anim.isUpdate = true;
				pStateCom.prevAnim = pStateCom.curAnim;
				if (pStateCom.prevState != pStateCom.curState)
				{
					switch (pStateCom.curState)
					{
					case CharacterStateComponent::State::Shooting:
						pStateCom.curAnim = "MoveDown"_hs;
						break;
					case CharacterStateComponent::State::Dead:
						pStateCom.curAnim = "MoveUp"_hs;
						break;
					default:
						break;
					}
				}

				if (pStateCom.curState == CharacterStateComponent::State::IdleAndWalking)
				{
					if (pStateCom.curDir == sf::Vector2i(0, 0))
					{
						anim.isUpdate = false;
					}
					else
					{
						if (pStateCom.curDir.x < 0)
						{
							pStateCom.curAnim = "MoveLeft"_hs;
						}
						else if(pStateCom.curDir.x > 0)
						{
							pStateCom.curAnim = "MoveRight"_hs;
						}
						if (pStateCom.curDir.y < 0)
						{
							pStateCom.curAnim = "MoveDown"_hs;
						}
						else if(pStateCom.curDir.y > 0)
						{
							pStateCom.curAnim = "MoveUp"_hs;
						}
					}
				}
				if (pStateCom.curAnim != pStateCom.prevAnim)
				{
					anim.curFrameTime = 0.0f;

					const auto& res = Codex<AnimationResource>::Retrieve(anim.animationName);
					const auto set = res.FindByHashedString(pStateCom.curAnim);
					anim.beginFrame = set.first;
					anim.iCurFrame = anim.beginFrame;
					anim.endFrame = set.second;
				}
			});
	}
};