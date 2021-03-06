#include "..\..\System\ControllerSystem.h"
#include "../../Component/PhysicComponent.h"
#include "../../Component/AbilityComponent.h"
#include "../../Component/AnimationComponent.h"
void ControllerSystem::BeginPlay(entt::registry& ECS) const
{
	ECS.set<PlayerController>();
}

void ControllerSystem::Update(entt::registry& ECS) const
{
	UpdatePlayerController(ECS);
}

void ControllerSystem::UpdatePlayerController(entt::registry& ECS) const
{
	//keyboard
	auto& kbd = ECS.ctx<Keyboard>();
	sf::Vector2i dir{ 0,0 };
	if (kbd.KeyIsPressed(sf::Keyboard::A))
	{
		dir.x -= 1;
	}
	if (kbd.KeyIsPressed(sf::Keyboard::D))
	{
		dir.x += 1;
	}
	if (kbd.KeyIsPressed(sf::Keyboard::S))
	{
		dir.y -= 1;
	}
	if (kbd.KeyIsPressed(sf::Keyboard::W))
	{
		dir.y += 1;
	}
	if (kbd.KeyIsPressed(sf::Keyboard::Space))
	{
		if (MaxxConsole::r_showDebugPhysic == 0)
		{
			MaxxConsole::r_showDebugPhysic = 1;
		}
		else
		{
			MaxxConsole::r_showDebugPhysic = 0;
		}
	}

	//mouse
	auto& mouse = ECS.ctx<Mouse>();
	auto& ct = ECS.ctx<PlayerController>();
	ct.mousePos = mouse.GetPos();

	ct.LeftIsPress = 0;
	ct.RightIsPress = 0;
	ct.direction = dir;
	while (!mouse.IsEmpty())
	{
		auto e = mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			ct.LeftIsPress++;
			continue;
		}
		if (e.GetType() == Mouse::Event::Type::RPress)
		{
			ct.RightIsPress++;
			continue;
		}
	}
	float speed = 16.0f;
	if (ECS.has<MontagePlaying>(ct.entity))
	{
		speed = 0.0f;
	}
	auto& al = ECS.get<GAS::AbilitySlot>(ct.entity);
	if (ct.LeftIsPress > 0)
	{
		ECS.assign<GAS::TryActivateAbility>(al.abilities[0]);
	}
	if (ct.RightIsPress > 0)
	{
		ECS.assign<GAS::TryActivateAbility>(al.abilities[1]);
	}
	auto& pc = ECS.get<Physic::Component>(ct.entity);
	b2Vec2 vel = pc->GetLinearVelocity();
	b2Vec2 desiredVel{ (float)ct.direction.x, (float)ct.direction.y };
	desiredVel.Normalize();
	desiredVel = speed * desiredVel;
	pc->ApplyLinearImpulseToCenter(pc->GetMass() * (desiredVel - vel), true);
}
