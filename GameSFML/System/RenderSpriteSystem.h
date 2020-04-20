#pragma once
#include "../System/ISystemECS.h"
#include "../System/IDrawSystem.h"
#include "../Component/PhysicComponent.h"
#include "../Component/AnimationComponent.h"
#include "../HashStringDataBase.h"
#include "entt/entt.hpp"
#include <algorithm>
#include <execution>
#include <iostream>
class SpirteUpdateSystem final : public ISystemECS
{
public:
	//update before Culling, animation, physic
	void Update(entt::registry& ECS)const  final
	{
		{
			auto view = ECS.view<AnimationComponent, sf::Sprite>();

			std::for_each(std::execution::par, view.begin(), view.end(), [&view](auto entity) {
				auto [animCom, sprite] = view.get<AnimationComponent, sf::Sprite>(entity);
				sprite.setTextureRect(Codex<AnimationResource>::Retrieve(animCom.animationName).GetRectByIndex(animCom.iCurFrame));
			});
		}

		{
			auto& gfx = ECS.ctx<Graphics>();
			auto view = ECS.view<Physic::Component, sf::Sprite>();
			std::for_each(std::execution::par, view.begin(), view.end(), [&view,&gfx](auto entity) {
				auto [physic, sprite] = view.get<Physic::Component, sf::Sprite>(entity);

				sprite.setPosition(gfx.WorldToScreenPos(physic->GetPosition()));
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