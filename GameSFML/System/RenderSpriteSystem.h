#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../System/IDrawSystem.h"
#include "../Component/PhysicComponent.h"
#include "../Component/AnimationComponent.h"
#include "../Component/GameplayTags.h"
#include "../HashStringDataBase.h"
#include "entt/entt.hpp"
#include <algorithm>
#include <execution>
#include <iostream>
class SpirteUpdateSystem final : public ISystemECS
{
public:
	//update before Culling, animation, physic
	void Update(entt::registry& ECS, float dt) final
	{
		{
			auto view = ECS.view<AnimationComponent, sf::Sprite>();

			std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
				auto& animCom = ECS.get<AnimationComponent>(entity);
				auto& sprite = ECS.get<sf::Sprite>(entity);
				sprite.setTextureRect(Codex<AnimationResource>::Retrieve(animCom.animationName).GetRectByIndex(animCom.iCurFrame));
			});
		}

		{
			auto view = ECS.view<PhysicComponent, sf::Sprite>();
			std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
				auto& sprite = ECS.get<sf::Sprite>(entity);

				sprite.setPosition(
					Locator::Graphic::ref().WorldToScreenPos(ECS.get<PhysicComponent>(entity).body->GetPosition())
				);
				});
		}
		
	}
};
class RenderSpriteSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::registry& ECS) const final
	{
		ECS.view<sf::Sprite>().each([&gfx](auto entity, sf::Sprite &sprite) {
			gfx.DrawSprite(sprite);
		});
	}
};