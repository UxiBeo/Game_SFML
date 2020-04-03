#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AttributeFactory.h"
#include "../Component/AttributeComponent.h"
class AttributeSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
public:
	static void ChangeSpecialStatKeepPercent(int& curValue, unsigned int& maxValue, const float newMaxValue)
	{
		float percent = (float)curValue / (float)maxValue;
		maxValue = (unsigned int)newMaxValue;
		curValue = int(percent * newMaxValue);
	}
	static float ComputeFinalValue(const AttributeFactory& factory, size_t attIndex, STATS::Set& set)
	{
		/*const auto& map = factory.GetSpecialMap(attIndex).dependentAtts;
		return std::accumulate(map.begin(), map.end(), 0.0f, [&set](float total, const AttributeFactory::DependIndex& index) {
			float finalvalue = set[index.index].getFinalValue();
			return total + finalvalue * index.perUnit + finalvalue * index.percent;
			});*/
		return 0.0f;
	}
	template<typename Value, typename Change>
	void ChangeValueSystem(entt::registry& ECS)
	{
		auto group = ECS.group<Value, const Change>();
		std::for_each(std::execution::par_unseq, group.begin(), group.end(), [&group](auto entity) {
			auto [base, change] = group.get<Value, const Change>(entity);

			for (auto& i : change)
			{
				atts[i.first] += i.second;
			}
			});
	}
};