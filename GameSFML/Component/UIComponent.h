#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"

struct ScreenBaseUI
{
	entt::entity ownerEntity;
	//this is the location offset from central of the screen
	sf::Vector2f offsetLocaion;
};
struct OwnedUIComponent
{
	//hold all Owned UI entity
	std::vector<entt::entity> entities;

	static void Destruction(entt::registry& ECS, entt::entity entity)
	{
		auto& owend = ECS.get<OwnedUIComponent>(entity);
		for (auto& e : owend.entities)
		{
			ECS.destroy(e);
		}
	}
};
struct WorldBaseUI
{
	entt::entity ownerEntity;
};

struct ProgressiveBarComponent
{
	sf::Vector2f size;
	float percent = 1.0f;
	sf::Color colorBase{ sf::Color::Green };
	sf::Color colorBG{ sf::Color::Red };
};
struct UpdateScreenBaseUIComponent
{
	entt::delegate<void(entt::entity, entt::registry&)> myDelegate;

	static void HealthText(entt::entity entity, entt::registry& ECS)
	{
		if (!ECS.has<sf::Text>(entity) || !ECS.has<ScreenBaseUI>(entity)) return;

		auto& UI = ECS.get<ScreenBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& text = ECS.get<sf::Text>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		std::stringstream ss;
		ss << "Health: " << (int)health.curHealth << " / " << (int)health.maxHealth;
		text.setString(ss.str());
		if (Locator::Graphic::empty()) return;

		text.setPosition(Locator::Graphic::ref().GetViewportLocation() + UI.offsetLocaion);
	}
	static void HealthBar(entt::entity entity, entt::registry& ECS)
	{
		if (!ECS.has<ProgressiveBarComponent>(entity) || !ECS.has<ScreenBaseUI>(entity)) return;

		auto& UI = ECS.get<ScreenBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& progressBar = ECS.get<ProgressiveBarComponent>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		progressBar.percent = health.curHealth / health.maxHealth;
	}
};
struct UpdateWorldBaseUIComponent
{
	entt::delegate<void(entt::entity, entt::registry&)> myDelegate;

	static void HealthText(entt::entity entity, entt::registry& ECS)
	{
		if (!ECS.has<sf::Text>(entity) || !ECS.has<WorldBaseUI>(entity)) return;

		auto& UI = ECS.get<WorldBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& text = ECS.get<sf::Text>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		std::stringstream ss;
		ss << (int)health.curHealth << " / " << (int)health.maxHealth;
		text.setString(ss.str());
	}
	static void HealthBar(entt::entity entity, entt::registry& ECS)
	{
		if (!ECS.has<ProgressiveBarComponent>(entity) || !ECS.has<WorldBaseUI>(entity)) return;

		auto& UI = ECS.get<WorldBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& progressBar = ECS.get<ProgressiveBarComponent>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		progressBar.percent = health.curHealth / health.maxHealth;
	}
};

struct UIFactory
{
	static entt::entity GetEntity(entt::entity entityOwner, entt::registry& ECS, bool bIsHaveOwner = true)
	{
		auto uiEntity = ECS.create();
		if (bIsHaveOwner)
		{

			if (ECS.has<OwnedUIComponent>(entityOwner))
			{
				auto& ownedUICom = ECS.get<OwnedUIComponent>(entityOwner);
				ownedUICom.entities.emplace_back(uiEntity);
				return uiEntity;
			}
			else
			{
				auto& ownedUICom = ECS.assign<OwnedUIComponent>(entityOwner);
				ownedUICom.entities.emplace_back(uiEntity);
				return uiEntity;
			}
		}

		return uiEntity;
	}
};