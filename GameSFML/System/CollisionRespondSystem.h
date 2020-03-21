#pragma once
#include "../Locator.h"
#include "../Component/PhysicComponent.h"
#include "../Component/HealthComponent.h"
#include "../System/ISystemECS.h"
#include <functional>

class PatternMatching
{
private:
	static const uint32_t GetHashCode(uint32_t type01, uint32_t type02)
	{
		return { type01 ^ (type02 + 0x9e3779b9 + (type01 << 6) + (type01 >> 2)) };
	}
public:
	template <class A, class B, class F>
	void AddCase(F function)
	{
		handlers[GetHashCode(A::value, B::value)] = function;
		handlers[GetHashCode(B::value, A::value)] = std::bind(
			function, std::placeholders::_1, std::placeholders::_3, std::placeholders::_2);
	}
	void SwitchCase(entt::hashed_string type01, entt::hashed_string type02, entt::registry& ECS, entt::entity entity01, entt::entity entity02)
	{
		auto i = handlers.find(GetHashCode(type01.value(), type02.value()));
		if (i != handlers.end())
		{
			i->second(ECS, entity01, entity02);
		}
		else
		{
			assert(false);
		}
	}
private:
	std::unordered_map<uint32_t, std::function<void(entt::registry & ECS, entt::entity, entt::entity)>> handlers;
};


class CollisionRespondSystem : public ISystemECS
{
	PatternMatching pattern;
	void Update(entt::registry& ECS, float dt) final
	{
		if (auto* listner = ECS.try_ctx<Box2DContactListener>(); listner)
		{
			const auto& data = listner->ReadData();
			for (size_t i = 0; i < data.size(); i++)
			{
				pattern.SwitchCase(ECS.get<GameplayTag>(data[i].first).tag, ECS.get<GameplayTag>(data[i].second).tag, ECS, data[i].first, data[i].second);
			}
			listner->ClearData();
		}
	}
public:
	CollisionRespondSystem()
	{
		pattern.AddCase<entt::tag<"Player"_hs>, entt::tag<"Enemy"_hs>>([](entt::registry& ECS, entt::entity entity01, entt::entity entity02) {


			});
		pattern.AddCase<entt::tag<"Bullet"_hs>, entt::tag<"Enemy"_hs>>([](entt::registry& ECS, entt::entity entity01, entt::entity entity02) {


			});
	}
};