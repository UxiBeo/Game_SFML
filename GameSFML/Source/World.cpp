#pragma once
#include "../World.h"
#include "../SystemInclude.h"
#include "../HashStringDataBase.h"
#include "../Component/CharacterStateComponent.h"
#include "../Map.h"
#include <random>
#include "../Grid.h"
#include "../JPS.h"
#include "../Component/StampContex.h"
#include "../Component/PrefapRegistry.h"
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
}

void World::AddNewPlayer(entt::registry& ECS)
{
	auto entity = ECS.create();
	ECS.ctx<PlayerController>().entity = entity;
	ECS.assign<TargetPosition>(entity);
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
	constexpr auto sa = "Data\\Json\\A_Shield.json"_hs;

	auto& animation = ECS.assign<AnimationComponent>(entity, sa);
	ECS.assign<FSM::State>(entity).mrD.connect<&PlayerAnimationState::Idle>();
	//sprite
	{
		auto& sprite = ECS.assign<sf::Sprite>(entity);
		sprite.setTexture(Codex<TextureResource>::Retrieve(animation.ar->textureName).data);
		//const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
		//sprite.setOrigin(textSize);
	}
	PhysicSystem::AddPhysic(entity, ECS, bodyDef, fixtureDef);
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
	ECS.set<StampContex>();
	ECS.set<PrefapRegistry>();
	ECS.set<PlayerController>();
}

void World::InitSystem()
{
	AddECSSystem(std::make_unique<WorldTimerSystem>());
	//AddECSSystem(std::make_unique<SpawnStaticObjectSystem>());
	//AddECSSystem(std::make_unique<LifeTimeSystem>());
	AddECSSystem(std::make_unique<ControllerSystem>());
	AddECSSystem(std::make_unique<StateMachineSystem>());
	AddECSSystem(std::make_unique<PhysicSystem>());

	AddECSSystem(std::make_unique<MoveCameraSystem>());
	AddECSSystem(std::make_unique<AbilitySystem>());
	AddECSSystem(std::make_unique<GameplayEffectSystem>());
	AddECSSystem(std::make_unique<AttributeSystem>());
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
