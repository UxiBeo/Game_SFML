#pragma once
#include "../Component/PhysicComponent.h"
#include "../System/ISystemECS.h"

class PhysicSystem final : public ISystemECS
{
	void BeginPlay(entt::registry& ECS) const final;
	void Update(entt::registry& ECS) const final;
public:
	static void AddPhysic(entt::entity entity, entt::registry& ECS, const b2BodyDef& Bdef, const b2FixtureDef& fixDef);

private:
	void ReactPhysic(entt::registry& ECS) const;
	static void DestroyPhysicComponent(entt::registry& ECS, entt::entity entity);
};

namespace Physic
{
	class ContactListener final : public b2ContactListener
	{
	public:
		//2 entity collided waith each other
		using PairEntity = std::pair<entt::entity, entt::entity>;

		void BeginContact(b2Contact* contact) final;
		void EndContact(b2Contact* contact) final;

		void ClearAll();
		const std::array<std::vector<PairEntity>, 3>& ReadData() const
		{
			return data;
		}
	private:
		//0: for normal Collision, 1: for sensor in, 2: for sensor out
		std::array<std::vector<PairEntity>, 3> data;
	};
}
