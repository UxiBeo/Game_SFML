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
#include "../Component/Ability/MeleeAttack.h"
#include "../Component/Ability/ProjectileAttack.h"
World::World(entt::registry& ECS)
{
	InitContex(ECS);
	InitSystem();
	for (auto& sys : ecsSystems)
	{
		sys->BeginPlay(ECS);
	}
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
	for (auto& sys : drawSystems)
	{
		sys->Draw(gfx, ECS);
	}
}

void World::AddNewPlayer(entt::registry& ECS)
{
	const auto entity = ECS.create();
	ECS.ctx<PlayerController>().entity = entity;
	ECS.assign<TargetPosition>(entity);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	b2PolygonShape box;
	
	box.SetAsBox(30.0f / 20.0f, 58.0f / 20.0f, { 0.0f, 53 / 20.0f }, 0.0f);
	auto& rect = ECS.assign<sf::RectangleShape>(entity);
	rect.setSize({ 60.0f, 116.0f });
	rect.setOrigin(30.0f, 107.0f);
	rect.setFillColor(sf::Color(150, 50, 250, 128));
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.filter.categoryBits = Physic::Fillter::PLAYER;
	//fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::STATIC;
	//fixtureDef.isSensor = isSensor;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 1.0f;
	constexpr auto sa = "Data\\Json\\a_greatsword.json"_hs;
	auto& animation = ECS.assign<AnimationComponent>(entity, sa);
	ECS.assign<FSM::State>(entity).mrD.connect<&PlayerAnimationState::Idle>();

	//meleeAttack
	{
		float cd = animation.frameTime * float(1 + animation.ar->sets[4].iEnd - animation.ar->sets[4].iBegin);
		const auto eac = ECS.create();
		auto& ac = ECS.assign<GAS::AbilityComponent>(eac);
		ac.owner = entity;
		ac.self = eac;
		ac.mrD.connect<&MeleeAttack::OnAbilityStart>();
		auto& ma = ECS.assign<MeleeAttack>(eac);
		ma.triggerTime = cd * 0.5f;
		ECS.assign<GAS::CooldownComponent>(eac).maxTime = cd;
		ECS.assign<GAS::AbilitySlot>(entity).abilities.emplace_back(eac);
		
	}
	
	//projectileAttack
	{
		entt::hashed_string animationName{ "Data\\Json\\a_effect.json"_hs };
		Codex<AnimationResource>::LoadFromFile(animationName);
		float cd = animation.frameTime * float(1 + animation.ar->sets[4 * 2].iEnd - animation.ar->sets[4 * 2].iBegin);
		const auto eac = ECS.create();
		auto& ac = ECS.assign<GAS::AbilityComponent>(eac);
		ac.owner = entity;
		ac.self = eac;
		ac.mrD.connect<&ProjectileAttack::OnAbilityStart>();
		auto& ma = ECS.assign<ProjectileAttack>(eac);
		ma.animationName = animationName;
		ma.castAnimation = 2;
		ma.projectileAnimation = 0;
		ma.triggerTime = cd * 0.5f;
		ECS.assign<GAS::CooldownComponent>(eac).maxTime = cd;
		ECS.get<GAS::AbilitySlot>(entity).abilities.emplace_back(eac);
	}

	ECS.assign<Tag::Bitfiled>(entity);
	//sprite
	{
		ECS.assign<sf::Sprite>(entity).setTexture(Codex<TextureResource>::Retrieve(animation.ar->textureName).data);
	}
	PhysicSystem::AddPhysic(entity, ECS, bodyDef, fixtureDef);
}

void World::AddECSSystem(std::unique_ptr<ISystemECS> newSystem)
{
	ecsSystems.emplace_back(std::move(newSystem));
}

void World::AddDrawSystem(std::unique_ptr<IDrawSystem> newSystem)
{
	drawSystems.emplace_back(std::move(newSystem));
}

void World::InitContex(entt::registry& ECS)
{
	ECS.set<std::mt19937>(std::random_device{}());
	ECS.set<StampContex>();
	ECS.set<PrefapRegistry>();
	ECS.set<TagRule>();
}

void World::InitSystem()
{
	AddECSSystem(std::make_unique<WorldTimerSystem>());
	AddECSSystem(std::make_unique<ImguiUpdateSystem>());
	AddECSSystem(std::make_unique<ControllerSystem>());
	AddECSSystem(std::make_unique<AbilitySystem>());
	AddECSSystem(std::make_unique<StateMachineSystem>());
	AddECSSystem(std::make_unique<PhysicSystem>());
	AddECSSystem(std::make_unique<MoveCameraSystem>());
	AddECSSystem(std::make_unique<GameplayEffectSystem>());
	AddECSSystem(std::make_unique<AttributeSystem>());
	AddECSSystem(std::make_unique<ParentChildrenSystem>());
	AddECSSystem(std::make_unique<CleanDeathSystem>());
	AddECSSystem(std::make_unique<AnimationSystem>());
	AddECSSystem(std::make_unique<SpirteUpdateSystem>());


	
	AddDrawSystem(std::make_unique<RenderSpriteSystem>());
	AddDrawSystem(std::make_unique<DrawDebugSystem>());
	AddDrawSystem(std::make_unique<ImguiDrawSystem>());
}
