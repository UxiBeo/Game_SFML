#pragma once
#include "entt/entt.hpp"

struct SpawnComponent
{
	bool bIsEnable = true;
	float curTime = 0.0f;
	float interval;
};
struct SpawnCapacity
{
	unsigned int totalEntity = 0;
	unsigned int maxEntity;
};
struct SpawnPosition
{
	b2Vec2 spawnPosition;
	b2Vec2 speed;
	unsigned int variationX;
	unsigned int variationY;
};
struct NotifyOnDead
{
	entt::entity onwerEntity;
	entt::sigh<void(entt::entity, entt::registry& ECS)> mySignal;
	static void EnemyNotifySpawner(entt::entity entity, entt::registry& ECS)
	{
		auto& notifier = ECS.get<NotifyOnDead>(entity);
		if (ECS.has<SpawnCapacity>(notifier.onwerEntity))
		{
			ECS.get<SpawnCapacity>(notifier.onwerEntity).totalEntity--;
		}
	}
};
struct UpdateSpawnComponent
{
	entt::delegate<void(entt::entity, entt::registry&)> myDelegate;

	static void Enemy(entt::entity spawnerEntity, entt::registry& ECS)
	{
		if (ECS.has<SpawnCapacity>(spawnerEntity))
		{
			auto& spawnCap = ECS.get<SpawnCapacity>(spawnerEntity);
			if (spawnCap.totalEntity >= spawnCap.maxEntity) return;
			spawnCap.totalEntity++;
		}

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;

		if (ECS.has<SpawnPosition>(spawnerEntity))
		{
			auto& spawnPos = ECS.get<SpawnPosition>(spawnerEntity);
			if (Locator::Random::empty() && (spawnPos.variationX == 0 && spawnPos.variationX == 0))
			{
				bodyDef.position = spawnPos.spawnPosition;
				bodyDef.linearVelocity = spawnPos.speed;
			}
			else
			{
				auto& rng = Locator::Random::ref();
				std::uniform_real_distribution<float> rangeX(-(float)spawnPos.variationX, (float)spawnPos.variationX);
				std::uniform_real_distribution<float> rangeY(-(float)spawnPos.variationY, (float)spawnPos.variationY);

				bodyDef.position = spawnPos.spawnPosition + b2Vec2(rangeX(rng), rangeY(rng));
				bodyDef.linearVelocity = spawnPos.speed;
			}
			
		}

		const float size = 1.3f;
		b2CircleShape circle;
		circle.m_radius = size;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;
		fixtureDef.filter.categoryBits = CollisionFillter::ENEMY;
		fixtureDef.filter.maskBits = CollisionFillter::PLAYER | CollisionFillter::STATIC | CollisionFillter::BULLET;
		//fixtureDef.isSensor = isSensor;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;

		auto entity = ECS.create();
		auto& notifier = ECS.assign<NotifyOnDead>(entity);
		notifier.onwerEntity = spawnerEntity;
		//notifier.mySignal.<&NotifyOnDead::EnemyNotifySpawner>();
		ECS.assign<HealthComponent>(entity, 50.0f, 50.0f);

		//sprite
		{
			std::uniform_int_distribution<int> textID(0, 2);
			const sf::Texture* texture;
			switch (textID(Locator::Random::ref()))
			{
			case 0:
				texture = &Codex<TextureResource>::Retrieve(Database::TEnemy01).data;
				break;
			case 1:
				texture = &Codex<TextureResource>::Retrieve(Database::TEnemy02).data;
				break;
			case 2:
				texture = &Codex<TextureResource>::Retrieve(Database::TEnemy03).data;
				break;
			default:
				texture = &Codex<TextureResource>::Retrieve(Database::TEnemy01).data;
				break;
			}
			auto& sprite = ECS.assign<sf::Sprite>(entity, *texture);
			const auto textSize = sf::Vector2f(0.5f * (float)sprite.getTexture()->getSize().x, 0.5f * (float)sprite.getTexture()->getSize().y);
			sprite.setOrigin(textSize);
		}

		ECS.assign<PhysicDebug>(entity);
		//ECS.assign<PhysicComponent>(entity, entity, ECS, bodyDef, fixtureDef);
		//ECS.assign<CollisionRespondComponent>(entity).myDelegate.connect<&CollisionRespondComponent::Enemy>();

		//Health Bar
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			auto& progressBar = ECS.assign<ProgressiveBarComponent>(entityUI, sf::Vector2f(100.0f, 15.0f));
			progressBar.colorBase = sf::Color::Blue;
			ECS.assign<WorldBaseUI>(entityUI, entity);
			ECS.assign<UpdateWorldBaseUIComponent>(entityUI).myDelegate.connect<&UpdateWorldBaseUIComponent::HealthBar>();
		}
		//health Text
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			ECS.assign<sf::Text>(entityUI, "Health: ", Codex<FontResource>::Retrieve(Database::FontSplatch).data, 15);
			ECS.assign<WorldBaseUI>(entityUI, entity);
			ECS.assign<UpdateWorldBaseUIComponent>(entityUI).myDelegate.connect<&UpdateWorldBaseUIComponent::HealthText>();
		}
	}
};