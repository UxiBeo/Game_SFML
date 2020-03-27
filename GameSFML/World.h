#pragma once
#include "HashStringDataBase.h"
#include "SystemInclude.h"
#include "Component/StaticFunction.h"
#include <random>
class World
{
public:
	World()
	{
		InitServiceLocator();
		InitSystem();
		//TestSomething();
		AddNewPlayer(Locator::ECS::ref());
		int aaa = 11111;
	}
	void Update()
	{
		if (Locator::ECS::empty()) return;
		auto& ECS = Locator::ECS::ref();
		for (auto& sys : ecsSystems)
		{
			sys->Update(ECS);
		}
	}
	void Draw()const
	{
		if (Locator::Graphic::empty() || Locator::ECS::empty()) return;
		auto& gfx = Locator::Graphic::ref();
		auto& ECS = Locator::ECS::ref();
		for (auto& sys : drawSystems)
		{
			sys->Draw(gfx, ECS);
		}
		
	}
	void AddNewPlayer(entt::registry& ECS) {
		auto entity = ECS.create();
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		const float size = 1.3f;
		b2CircleShape circle;
		circle.m_radius = size;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;
		fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
		//fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::STATIC;
		//fixtureDef.isSensor = isSensor;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;
		
		
		//ECS.assign<HealthComponent>(entity, 50.0f, 50.0f);
		ECS.assign<PlayerControllerComponent>(entity);

		auto& animation = ECS.assign<AnimationComponent>(entity, Database::PlayerAnimation);
		ECS.assign<CharacterStateComponent>(entity);
			//sprite
		{
			auto& sprite = ECS.assign<sf::Sprite>(entity);
			const auto textureName = Codex<AnimationResource>::Retrieve(animation.animationName).textureName;
			sprite.setTexture(Codex<TextureResource>::Retrieve(textureName).data);
			//const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
			//sprite.setOrigin(textSize);
		}


		//ECS.assign<PhysicDebug>(entity);
		ECS.assign<entt::tag<"CameraTracking"_hs>>(entity);
		Sfunc::AddPhysic(entity, ECS, bodyDef, fixtureDef);
		
		//ECS.assign<CollisionRespondComponent>(entity).myDelegate.connect<&CollisionRespondComponent::Player>();

		//health text
		/*{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			ECS.assign<sf::Text>(entityUI, "Health: ", Codex<FontResource>::Retrieve(Database::FontSplatch).data, 50);
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateScreenBaseUIComponent::HealthText>();
		}*/


		//Health Bar
		/*{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			auto& progressBar = ECS.assign<ProgressiveBarComponent>(entityUI, sf::Vector2f(400.0f, 55.0f));
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateScreenBaseUIComponent::HealthBar>();
		}*/
	}
	void AddECSSystem(std::unique_ptr<ISystemECS> newSystem)
	{
		ecsSystems.emplace_back(std::move(newSystem));
	}
	void AddDrawSystem(std::unique_ptr<IDrawSystem> newSystem)
	{
		drawSystems.emplace_back(std::move(newSystem));
	}
	void AddSpawner(entt::registry& ECS, float worldSize)
	{
		auto entity = ECS.create();
		ECS.assign<SpawnComponent>(entity).interval = 1.0f;
		ECS.assign<SpawnCapacity>(entity).maxEntity = 20;
		ECS.assign<UpdateSpawnComponent>(entity).myDelegate.connect<UpdateSpawnComponent::Enemy>();

		if (Locator::Random::empty()) return;
		auto& rng = Locator::Random::ref();
		std::uniform_real_distribution<float> posRange(-worldSize, worldSize);
		std::uniform_real_distribution<float> speedRange(-20.0f, 20.0f);
		auto& spawnPos = ECS.assign<SpawnPosition>(entity);
		spawnPos.spawnPosition = b2Vec2(posRange(rng), posRange(rng));
		spawnPos.variationX = 5;
		spawnPos.variationX = 2;
		spawnPos.speed = b2Vec2(speedRange(rng), speedRange(rng));
	}
	void AddPlayer(entt::registry& ECS)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		const float size = 1.3f;
		b2CircleShape circle;
		circle.m_radius = size;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;
		fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
		//fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::STATIC;
		//fixtureDef.isSensor = isSensor;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;
		auto entity = ECS.create();
		//ECS.assign<HealthComponent>(entity, 50.0f, 50.0f);
		ECS.assign<PlayerControllerComponent>(entity);

		/*auto& animation = ECS.assign<AnimationComponent>(entity,
			Codex<AnimationResource>::Retrieve(Database::PlayerAnimation),
			ECS.assign<PlayerStateComponent>(entity).state);*/

		//sprite
		{
			auto& sprite = ECS.assign<sf::Sprite>(entity);
			//sprite.setTexture(*animation.resource->texture);
			//const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
			//sprite.setOrigin(textSize);
		}


		ECS.assign<entt::tag<"PhysicDebug"_hs>>(entity);
		ECS.assign<entt::tag<"CameraTracking"_hs>>(entity);
		//ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
		//ECS.assign<CollisionRespondComponent>(entity).myDelegate.connect<&CollisionRespondComponent::Player>();

		//health text
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			ECS.assign<sf::Text>(entityUI, "Health: ", Codex<FontResource>::Retrieve(Database::FontSplatch).data, 50);
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateScreenBaseUIComponent::HealthText>();
		}
		

		//Health Bar
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			auto& progressBar = ECS.assign<ProgressiveBarComponent>(entityUI, sf::Vector2f(400.0f, 55.0f));
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateScreenBaseUIComponent::HealthBar>();
		}
	}
private:
	void SignalComponent(entt::registry& ECS)
	{
		ECS.on_destroy<PhysicComponent>().connect<&Sfunc::DestroyPhysicComponent>();
	}
	template<typename Context>
	Context& AddContex(entt::registry& ECS)
	{
		return ECS.set<Context>();
	}
	void InitServiceLocator()
	{
		Locator::Random::set(std::random_device{}());
		Locator::ECS::set();
		SignalComponent(Locator::ECS::ref());
		if (!Locator::ECS::empty())
		{
			auto& ECS = Locator::ECS::ref();
			ECS.set<PhysicEngine>(b2Vec2(0.0f,0.0f)).SetContactListener(&ECS.set<Box2DContactListener>());

			ECS.set<Grid>().LoadFromFile(Database::GridMap);
			ECS.set<WorldTimer>();
		}
		
	}
	void InitSystem()
	{
		AddECSSystem(std::make_unique<WorldTimerSystem>());
		//AddECSSystem(std::make_unique<SpawnStaticObjectSystem>());
		//AddECSSystem(std::make_unique<LifeTimeSystem>());
		AddECSSystem(std::make_unique<PlayerControllerSystem>());
		AddECSSystem(std::make_unique<PlayerUpdateSystem>());
		AddECSSystem(std::make_unique<StateMachineSystem>());
		AddECSSystem(std::make_unique<PhysicSystem>());
		AddECSSystem(std::make_unique<CollisionRespondSystem>());
		AddECSSystem(std::make_unique<AttributeSystem>());
		AddECSSystem(std::make_unique<MoveCameraSystem>());
		//AddECSSystem(std::make_unique<GridUpdateSystem>());
		//AddECSSystem(std::make_unique<SpawnSystem>());
		AddECSSystem(std::make_unique<CleanDeathSystem>());
		//AddECSSystem(std::make_unique<CullingSystem>());
		//AddECSSystem(std::make_unique<UpdateScreenBaseUISystem>());
		//AddECSSystem(std::make_unique<UpdateWorldBaseUISystem>());
		//AddECSSystem(std::make_unique<TransitionStateSystem>());
		AddECSSystem(std::make_unique<AnimationSystem>());
		AddECSSystem(std::make_unique<SpirteUpdateSystem>());

		//render Grid should be before render sprite and after move camera;
		AddDrawSystem(std::make_unique<RenderGridSystem>());
		//render Sprite should be the last
		AddDrawSystem(std::make_unique<RenderSpriteSystem>());
		//drawSystems.emplace_back(std::make_unique<DrawDebugSystem>());
		//drawSystems.emplace_back(std::make_unique<RenderWorldBaseUISystem>());
		//drawSystems.emplace_back(std::make_unique<RenderScreenBaseUISystem>());
	}
	void AddWall(b2Vec2 p1, b2Vec2 p2)
	{
		const auto entity = Locator::ECS::ref().create();
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);

		b2EdgeShape edgeShape;
		edgeShape.Set(p1, p2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &edgeShape;

		//Locator::ECS::ref().assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
	}
	void TestSomething()
	{
		auto& ECS = Locator::ECS::ref();
		auto& rng = Locator::Random::ref();
		const float worldSize = 100.0f;
		AddWall(b2Vec2(worldSize, -worldSize), b2Vec2(worldSize, worldSize));
		AddWall(b2Vec2(worldSize, worldSize), b2Vec2(-worldSize, worldSize));
		AddWall(b2Vec2(-worldSize, worldSize), b2Vec2(-worldSize, -worldSize));
		AddWall(b2Vec2(-worldSize, -worldSize), b2Vec2(worldSize, -worldSize));

		AddPlayer(ECS);

		std::uniform_real_distribution<float> pos(-worldSize + 5.0f, worldSize - 5.0f);
		std::uniform_real_distribution<float> speed(-20.0f, 20.0f);
		for (size_t i = 0; i < 10; i++)
		{
			AddSpawner(ECS, worldSize - 10.0f);
		}
	}
private:
	std::vector<std::unique_ptr<ISystemECS>> ecsSystems;
	std::vector<std::unique_ptr<IDrawSystem>> drawSystems;
};