#pragma once
#include "../../System/AttributeSystem.h"
#include <future>
#include <vector>
void AttributeSystem::Update(entt::registry& ECS)
{
	//std::vector<std::future<void>> futures;
	//futures.push_back(std::async(&AttributeSystem::ModifiedStat<RPGS::ENormal, RPGS::Value, RPGS::Value>, this, std::ref(ECS));
	ModifiedStat<RPGS::ENormal, RPGS::Value, RPGS::Value>(ECS);
	ModifiedStat<RPGS::EDependent, RPGS::Value, RPGS::Value>(ECS);
	ModifiedStat<RPGS::EConsume, RPGS::ConsumeValue, RPGS::ConsumeValue>(ECS);
	ModifiedStat<RPGS::EConsume, RPGS::ConsumeValue, RPGS::Value>(ECS);
	ModifiedStat<RPGS::EConsume, RPGS::ConsumeValue, RPGS::CurrentConsume>(ECS);

	ClearModified<RPGS::ENormal, RPGS::Value>(ECS);
	ClearModified<RPGS::EDependent, RPGS::Value>(ECS);
	ClearModified<RPGS::EConsume, RPGS::ConsumeValue>(ECS);
	ClearModified<RPGS::EConsume, RPGS::Value>(ECS);
	ClearModified<RPGS::EConsume, RPGS::CurrentConsume>(ECS);
}

void AttributeSystem::BeginPlay(entt::registry& ECS)
{
	ModifiedStat<RPGS::ENormal, RPGS::Value, RPGS::Value>(ECS);
	ModifiedStat<RPGS::EDependent, RPGS::Value, RPGS::Value>(ECS);
	ModifiedStat<RPGS::EConsume, RPGS::ConsumeValue, RPGS::ConsumeValue>(ECS);
	ModifiedStat<RPGS::EConsume, RPGS::ConsumeValue, RPGS::Value>(ECS);
	ModifiedStat<RPGS::EConsume, RPGS::ConsumeValue, RPGS::CurrentConsume>(ECS);
}
/*
void AttributeSystem::ChangeConsume(entt::registry& ECS) const
{
	auto group = ECS.group<STATS::Consume, const STATS::ChangeConsume>();
	std::for_each(std::execution::par_unseq, group.begin(), group.end(), [&group](auto entity) {

		auto [base, change] = group.get<STATS::Consume, const STATS::ChangeConsume>(entity);

		std::for_each(std::execution::par_unseq, change.data.begin(), change.data.end(), [&base](auto& c) {

			auto& data = base.curValue[c.first];
			data += (int)c.second;

			});
		});
}

void AttributeSystem::HealthReachZero(entt::registry& ECS) const
{
	char HealthIndex = ECS.ctx<STATS::Map>().GetHealthIndex();
	ECS.group<const STATS::Consume, const STATS::ChangeBaseConsume>().each([&ECS, HealthIndex](auto entity, const STATS::Consume& consume, const auto&) {
		if (consume.curValue[HealthIndex] <= 0)
			ECS.assign<STATS::ReachZero>(entity);
		});
}

void AttributeSystem::ClearChange(entt::registry& ECS) const
{
	ECS.clear<STATS::ChangeBasic>();
	ECS.clear<STATS::ChangeConsume>();
	ECS.clear<STATS::ChangeBaseConsume>();
}

void AttributeSystem::ChangeBaseConsume(entt::registry& ECS) const
{
	auto group = ECS.group<STATS::Consume, const STATS::ChangeBaseConsume>();
	std::for_each(std::execution::par_unseq, group.begin(), group.end(), [&group](auto entity) {

		auto [base, change] = group.get<STATS::Consume, const STATS::ChangeBaseConsume>(entity);

		std::for_each(std::execution::par_unseq, change.data.begin(), change.data.end(), [&base](auto& c) {
			auto& data = base.data[c.first];
			data += c.second;
			int& curValue = base.curValue[c.first];
			float& curMax = base.maxValue[c.first];
			float prevMax = curMax;
			curMax = data.getFinalValue();
			curValue = int((float)curValue / prevMax * curMax);
			});
		});
}

void AttributeSystem::ChangeBasic(entt::registry& ECS) const
{
	auto group = ECS.group<STATS::Basic, const STATS::ChangeBasic>();
	std::for_each(std::execution::par_unseq, group.begin(), group.end(), [&group](auto entity) {

		auto [base, change] = group.get<STATS::Basic, const STATS::ChangeBasic>(entity);

		std::for_each(std::execution::par_unseq, change.data.begin(), change.data.end(), [&base](auto& c) {

			base.data[c.first] += c.second;

			});
		});
}
*/