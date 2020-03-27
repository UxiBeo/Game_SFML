#pragma once
#include "../Locator.h"
#include "../Component/PhysicComponent.h"
#include "../Component/CollisionRespondComponent.h"
#include "../System/ISystemECS.h"
class CollisionRespondSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) final
	{
		/*---------------Normal Collision----------------*/
		{
			auto view = ECS.view<const ColliedWith, const FColliedWithPar>();
			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view,&ECS](auto entity) {

				auto [colliedWith, respond] = view.get<const ColliedWith, const FColliedWithPar>(entity);

				if (respond) respond(entity, colliedWith, ECS);
				});
		}

		ECS.view<ColliedWith, FColliedWithSeg>().each([&ECS](auto entity, const auto& data, const auto& respond) {
			
			if (respond) respond(entity, data, ECS);

			});
		ECS.clear<ColliedWith>();
		/*---------------Normal Collision----------------*/


		/*-------------------Sensor Out------------------*/
		ECS.group<SensorOut>(entt::get<SensorWith>).each([&ECS](auto entity, const auto& senOut, auto& senWith) {

			SensorWith diff;
			std::set_difference(senWith.data.begin(), senWith.data.end(), senOut.data.begin(), senOut.data.end(), std::back_inserter(diff.data));

			if ((senWith.data.size() - diff.data.size()) != senOut.data.size()) assert(false && "Sensor out have some element mismatch");

			if (auto* fSensor = ECS.try_get<FSensorOut>(entity); fSensor)
			{
				if (fSensor->func)
					fSensor->func(entity, senOut.data, ECS);
			}

			if (diff.data.size() != 0)
			{
				senWith.data = std::move(diff.data);
			}
			else
			{
				ECS.remove<SensorWith>(entity);
			}
			
			
			});
		ECS.clear<SensorOut>();
		/*-------------------Sensor Out------------------*/


		/*-------------------Sensor In------------------*/
		ECS.view<SensorIn>().each([&ECS](auto entity, const auto& senIn) {

			auto& senWith = ECS.get_or_assign<SensorWith>(entity);
			SensorWith merge;
			std::merge(senWith.data.begin(), senWith.data.end(), senIn.data.begin(), senIn.data.end(), std::back_inserter(merge.data));

			if ((senWith.data.size() + senIn.data.size()) != merge.data.size()) assert(false && "Sensor out have some element mismatch");

			if (auto* fSensor = ECS.try_get<FSensorIn>(entity); fSensor && fSensor->func)
			{
					fSensor->func(entity, senIn.data, ECS);
			}

			senWith.data = std::move(merge.data);
			});
		ECS.clear<SensorIn>();
		/*-------------------Sensor In------------------*/
		

		/*-------------------Sensor With------------------*/
		auto group = ECS.group<const FSensorPar>(entt::get<const SensorWith>);
		std::for_each(std::execution::par_unseq, group.begin(), group.end(), [&group, &ECS](auto entity) {

			auto [fSensor, sensorWith] = group.get<const FSensorPar, const SensorWith>(entity);
			if (fSensor.func)
				fSensor.func(entity, sensorWith.data, ECS);
			
			});

		ECS.group<FSensorSeg>(entt::get<const SensorWith>).each([&ECS](auto entity, const auto& respond, const auto& senWith) {

			if (respond.func) respond.func(entity, senWith.data, ECS);

			});
		/*-------------------Sensor With------------------*/
	}
};

//class PatternMatching
//{
//private:
//	static const uint32_t GetHashCode(uint32_t type01, uint32_t type02)
//	{
//		return { type01 ^ (type02 + 0x9e3779b9 + (type01 << 6) + (type01 >> 2)) };
//	}
//public:
//	template <class A, class B, class F>
//	void AddCase(F function)
//	{
//		handlers[GetHashCode(A::value, B::value)] = function;
//		handlers[GetHashCode(B::value, A::value)] = std::bind(
//			function, std::placeholders::_1, std::placeholders::_3, std::placeholders::_2);
//	}
//	void SwitchCase(entt::hashed_string type01, entt::hashed_string type02, entt::registry& ECS, entt::entity entity01, entt::entity entity02)
//	{
//		auto i = handlers.find(GetHashCode(type01.value(), type02.value()));
//		if (i != handlers.end())
//		{
//			i->second(ECS, entity01, entity02);
//		}
//		else
//		{
//			assert(false);
//		}
//	}
//private:
//	std::unordered_map<uint32_t, std::function<void(entt::registry & ECS, entt::entity, entt::entity)>> handlers;
//};
//
//
//class CollisionRespondSystem : public ISystemECS
//{
//	PatternMatching pattern;
//	void Update(entt::registry& ECS, float dt) final
//	{
//		if (auto* listner = ECS.try_ctx<Box2DContactListener>(); listner)
//		{
//			const auto& data = listner->ReadData();
//			for (size_t i = 0; i < data.size(); i++)
//			{
//				pattern.SwitchCase(ECS.get<GameplayTag>(data[i].first).tag, ECS.get<GameplayTag>(data[i].second).tag, ECS, data[i].first, data[i].second);
//			}
//			listner->ClearData();
//		}
//	}
//public:
//	CollisionRespondSystem()
//	{
//		pattern.AddCase<entt::tag<"Player"_hs>, entt::tag<"Enemy"_hs>>([](entt::registry& ECS, entt::entity entity01, entt::entity entity02) {
//
//
//			});
//		pattern.AddCase<entt::tag<"Bullet"_hs>, entt::tag<"Enemy"_hs>>([](entt::registry& ECS, entt::entity entity01, entt::entity entity02) {
//
//
//			});
//	}
//};