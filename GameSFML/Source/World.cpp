#pragma once
#include "../World.h"
#include "../SystemInclude.h"
#include "../HashStringDataBase.h"
#include "../Component/CharacterStateComponent.h"
#include "../Map.h"
#include <random>
World::World(entt::registry& ECS)
{
	InitContex(ECS);
	InitSystem();
	BeginPlay(ECS);
	AddNewPlayer(ECS);
}

void World::Update(entt::registry& ECS)
{
	for (auto& sys : ecsSystems)
	{
		sys->Update(ECS);
	}
}

void World::Draw(Graphics& gfx, entt::registry& ECS) const
{
	gfx.Draw(ECS.ctx<Map>());
	for (auto& sys : drawSystems)
	{
		sys->Draw(gfx, ECS);
	}
	auto controller = ECS.ctx<PlayerControllerComponent>();
	
	sf::CircleShape shape;
	shape.setRadius(32);
	
	
	shape.setPosition(gfx.getRenderWindow().mapPixelToCoords(controller.mouseScreenPos) - sf::Vector2f(32.0f,32.0f));
	shape.setFillColor(sf::Color(0, 0, 255, 128));
	gfx.Draw(shape);
}

void World::AddNewPlayer(entt::registry& ECS)
{
	auto entity = ECS.create();
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	const float size = 1.3f;
	b2CircleShape circle;
	circle.m_radius = size;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.filter.categoryBits = Physic::Fillter::PLAYER;
	//fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::STATIC;
	//fixtureDef.isSensor = isSensor;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 1.0f;


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
	PhysicSystem::AddPhysic(entity, ECS, bodyDef, fixtureDef);

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

void World::AddPlayer(entt::registry& ECS)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	const float size = 1.3f;
	b2CircleShape circle;
	circle.m_radius = size;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.filter.categoryBits = Physic::Fillter::PLAYER;
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

void World::BeginPlay(entt::registry& ECS)
{
	for (auto& sys : ecsSystems)
	{
		sys->BeginPlay(ECS);
	}
}

void World::InitContex(entt::registry& ECS)
{
	ECS.set<std::mt19937>(std::random_device{}());
	ECS.set<Physic::Engine>(b2Vec2(0.0f, 0.0f));
	//ECS.set<Grid>().LoadFromFile(Database::GridMap);
	ECS.set<Map>().Load("Data/Json/map200x200.tmx");
}

void World::InitSystem()
{
	AddECSSystem(std::make_unique<WorldTimerSystem>());
	//AddECSSystem(std::make_unique<SpawnStaticObjectSystem>());
	//AddECSSystem(std::make_unique<LifeTimeSystem>());
	AddECSSystem(std::make_unique<PlayerControllerSystem>());
	AddECSSystem(std::make_unique<PlayerUpdateSystem>());
	AddECSSystem(std::make_unique<StateMachineSystem>());
	AddECSSystem(std::make_unique<PhysicSystem>());
	AddECSSystem(std::make_unique<AbilitySystem>());
	AddECSSystem(std::make_unique<GameplayEffectSystem>());
	AddECSSystem(std::make_unique<AttributeSystem>());
	AddECSSystem(std::make_unique<MoveCameraSystem>());
	//AddECSSystem(std::make_unique<GridUpdateSystem>());
	AddECSSystem(std::make_unique<ParentChildrenSystem>());
	AddECSSystem(std::make_unique<CleanDeathSystem>());
	//AddECSSystem(std::make_unique<CullingSystem>());
	//AddECSSystem(std::make_unique<UpdateScreenBaseUISystem>());
	//AddECSSystem(std::make_unique<UpdateWorldBaseUISystem>());
	//AddECSSystem(std::make_unique<TransitionStateSystem>());
	AddECSSystem(std::make_unique<AnimationSystem>());
	AddECSSystem(std::make_unique<SpirteUpdateSystem>());

	//render Grid should be before render sprite and after move camera;
	//AddDrawSystem(std::make_unique<RenderGridSystem>());
	//render Sprite should be the last
	AddDrawSystem(std::make_unique<RenderSpriteSystem>());
	//drawSystems.emplace_back(std::make_unique<DrawDebugSystem>());
	//drawSystems.emplace_back(std::make_unique<RenderWorldBaseUISystem>());
	//drawSystems.emplace_back(std::make_unique<RenderScreenBaseUISystem>());
}
