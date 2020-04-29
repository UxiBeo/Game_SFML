#pragma once
#include "../AbilityComponent.h"
#include "../../System/PhysicSystem.h"
#include "../ControllerComponent.h"
#include "../AnimationComponent.h"
#include "../GameplayEffectComponent.h"
#include "../TimerComponent.h"
#include "../../Graphics.h"
struct ProjectileAttack
{
	static void OnAbilityStart(const GAS::AbilityComponent& ab, entt::registry& ECS)
	{
		ECS.assign<GAS::CommitAbility>(ab.self);
		ECS.assign<GAS::Activating>(ab.self);
		const auto& pa = ECS.get<ProjectileAttack>(ab.self);
		auto& ac = ECS.get<AnimationComponent>(ab.owner);
		auto& pm = ECS.get_or_assign<PlayAnimationMontage>(ab.owner);
		pm.an.triggerTime = pa.triggerTime;
		pm.an.listenerE = ab.self;
		pm.an.triggerD.connect<&ProjectileAttack::OnAnimationNotify>();
		pm.an.interrupD.connect<&ProjectileAttack::OnAnimationInterrupt>();
		pm.iSet = ac.iSet % 4 + 4 * pa.castAnimation;
	}
private:
	static void OnAnimationInterrupt(const entt::entity& owner, const entt::entity& self, entt::registry& ECS)
	{
		ECS.remove<GAS::Activating>(self);
	}
	static void OnAnimationNotify(const entt::entity& owner, const entt::entity& self, entt::registry& ECS)
	{
		auto& pe = ECS.ctx<Physic::Engine>();
		const auto& gp = ECS.get<TargetPosition>(owner);
		const auto& ac = ECS.get<AnimationComponent>(owner);
		const auto& me = ECS.get<ProjectileAttack>(self);
		float nLength = 4.0f;
		float length = nLength / sqrt(2.0f);
		b2Vec2 pos{ 0.0f,0.0f };
		switch (ac.iSet % 4)
		{
		case 0:
		{
			pos = me.attackOffsetLR;
		}
		break;
		case 1:
		{
			pos = me.attackOffsetD;
		}
		break;
		case 2:
		{
			pos = { -me.attackOffsetLR.x, me.attackOffsetLR.y };
		}
		break;
		case 3:
		{
			pos = me.attackOffsetU;
		}
		break;
		default:
			break;
		}

		pos += gp.curPos;

		auto eProjectile = ECS.create();
		auto& pAnim = ECS.assign<AnimationComponent>(eProjectile, me.animationName, me.projectileAnimation);
		ECS.assign<sf::Sprite>(eProjectile).setTexture(Codex<TextureResource>::Retrieve(pAnim.ar->textureName).data);
		ECS.assign<Timer::LifeTimeComponent>(eProjectile, 0.0f, 5.0f);
		//physic
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.fixedRotation = true;
			b2CircleShape circle;
			circle.m_radius = 1.0f;
			auto& rect = ECS.assign<sf::RectangleShape>(eProjectile);
			rect.setSize({ circle.m_radius * 20.0f, circle.m_radius * 20.0f });
			rect.setOrigin(circle.m_radius * 20.0f / 2.0f, circle.m_radius * 20.0f / 2.0f);
			rect.setFillColor(sf::Color(150, 150, 250, 128));
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circle;
			fixtureDef.filter.categoryBits = Physic::Fillter::PLAYER;
			//fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::STATIC;
			//fixtureDef.isSensor = isSensor;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.restitution = 1.0f;
			auto& pc = PhysicSystem::AddPhysic(eProjectile, ECS, bodyDef, fixtureDef);
			pc->SetTransform(pos, 0.0f);
			pc->ApplyLinearImpulseToCenter(pc->GetMass() * me.projectileSpeed * (gp.goalPos - pos), true);
			ECS.assign<Physic::Listener<Physic::Type::Normal>>(eProjectile).mrD.connect<&ProjectileAttack::OnProjectileHit>();
			ECS.assign<GAS::ModifiedEffect>(eProjectile);
		}
		ECS.remove<GAS::Activating>(self);
	}
	static void OnProjectileHit(entt::registry& ECS, const entt::entity self, const entt::entity other)
	{
		const auto& mes = ECS.get<GAS::ModifiedEffect>(self);
		for (auto& me : mes.eEffect)
		{
			auto& te = ECS.assign<GES::TryAppyEffect>(other);
			te.prefapEntity = me;
			te.source = self;
			te.target = other;
		}
	}
public:
	uint8_t castAnimation = 0;
	entt::hashed_string animationName;
	uint8_t projectileAnimation = 0;
	float triggerTime = 0.0f;
	float projectileSpeed = 3.0f;
	b2Vec2 attackOffsetLR = { 4.0f, 4.0f };
	b2Vec2 attackOffsetD = { 0.0f, -3.5f };
	b2Vec2 attackOffsetU = { 0.0f, 5.0f };
};