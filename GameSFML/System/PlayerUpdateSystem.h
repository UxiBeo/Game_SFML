#pragma once
#include "../System/ISystemECS.h"
#include "../Component/PlayerControllerComponent.h"
#include "../Component/PhysicComponent.h"
class PlayerUpdateSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) const final
	{
		auto& controller = ECS.ctx<PlayerControllerComponent>();
		//update controller
		ECS.view<entt::tag<"CameraTracking"_hs>, Physic::Component>().each([&ECS, &controller](auto entity, auto, Physic::Component& physic) {
			
			b2Vec2 vel = physic->GetLinearVelocity();
			b2Vec2 desiredVel{(float)controller.direction.x, (float)controller.direction.y };
			desiredVel.Normalize();
			desiredVel = 8.0f * desiredVel;
			physic->ApplyLinearImpulseToCenter(physic->GetMass() * (desiredVel - vel), true);

			//if (controller.bIsShooting)
			//{
			//	auto entityBullet = ECS.create();
			//	ECS.assign<LifeTimeComponent>(entityBullet, 5.0f, 0.0f);
			//	ECS.assign<HealthComponent>(entityBullet, 10.0f, 10.0f);
			//	//animation
			//	auto& animation = ECS.assign<AnimationComponent>(entityBullet, Codex<AnimationResource>::Retrieve(Database::BulletAnimation));
			//	b2CircleShape circle;
			//	//sprite
			//	{
			//		/*auto& sprite = ECS.assign<sf::Sprite>(entityBullet);
			//		sprite.setTexture(*animation.resource->texture);
			//		const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
			//		sprite.setOrigin(textSize);
			//		circle.m_radius = 0.8f * textSize.x / gfx.scalePixel;*/
			//	}

			//	//physic
			//	const auto parentPosition = physic.body->GetPosition();
			//	auto direction = gfx.MouseToWorldPos(controller.mousePos) - parentPosition;
			//	direction.Normalize();
			//	b2BodyDef bodyDef;
			//	bodyDef.type = b2_dynamicBody;
			//	bodyDef.position = parentPosition + direction;
			//	

			//	b2FixtureDef fixtureDef;
			//	fixtureDef.shape = &circle;
			//	fixtureDef.filter.categoryBits = CollisionFillter::BULLET;
			//	fixtureDef.filter.maskBits = CollisionFillter::ENEMY;
			//	//fixtureDef.isSensor = true;
			//	fixtureDef.density = 1.0f;
			//	fixtureDef.friction = 0.0f;
			//	fixtureDef.restitution = 1.0f;

			//	ECS.assign<PhysicDebug>(entityBullet);
			//	auto& bulletPhysic = ECS.assign<PhysicComponent>(entityBullet, entityBullet, bodyDef, fixtureDef);
			//	bulletPhysic.body->ApplyForceToCenter(5000.0f * direction, true);
			//	ECS.assign<CollisionRespondComponent>(entityBullet).myDelegate.connect<&CollisionRespondComponent::Bullet>();
			//}
		});
	}
};