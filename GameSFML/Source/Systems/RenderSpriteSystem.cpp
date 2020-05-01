#include "../../System/RenderSpriteSystem.h"
#include "../../Component/PhysicComponent.h"
#include "../../Component/AnimationComponent.h"
#include "../../GlowComponent.h"
#include "../../Map.h"
#include "imgui.h"
#include "entt/entt.hpp"
#include <algorithm>
#include <execution>

void SpirteUpdateSystem::Update(entt::registry& ECS) const
{
	{
		auto view = ECS.view<AnimationComponent, sf::Sprite>();
		std::for_each(view.begin(), view.end(), [&view](auto entity) {
			auto [ac, sprite] = view.get<AnimationComponent, sf::Sprite>(entity);
			sprite.setTextureRect(ac.ar->frames[ac.iCurrent].rect);
			sprite.setOrigin(ac.ar->frames[ac.iCurrent].pivot);
			});
	}
	{
		auto& gfx = ECS.ctx<Graphics>();
		auto view = ECS.view<Physic::Component, sf::Sprite>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&view, &gfx](auto entity) {
			auto [physic, sprite] = view.get<Physic::Component, sf::Sprite>(entity);
			sprite.setPosition(gfx.WorldToScreenPos(physic->GetPosition()));
			});
	}
}
void SpirteUpdateSystem::BeginPlay(entt::registry& ECS) const
{
	ECS.set<Map>().Load("Data/Json/map200x200.tmx");
}

void RenderSpriteSystem::Draw(Graphics& gfx, entt::registry& ECS) const
{
	gfx.Draw(ECS.ctx<Map>());
	ECS.view<sf::Sprite>().each([&gfx](auto entity, sf::Sprite& sprite) {
		gfx.DrawSprite(sprite);
		});
	/*auto& ge = ECS.ctx<GlowEffect>();

	gfx.getRenderWindow().draw(ge.sp3, ge.state);*/
}
