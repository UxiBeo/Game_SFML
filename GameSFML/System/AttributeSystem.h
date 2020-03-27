#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AttributeComponent.h"
#include "../Component/AttributeFactory.h"
class AttributeSystem final : public ISystemECS
{
public:
	void Update(entt::registry& ECS) final
	{
		if (auto* factory = ECS.try_ctx<AttributeFactory>(); factory)
		{
			//Attribute With min max value
			{
				auto view = ECS.view<const ChangeAttribute, AttributeSet>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &ECS, &factory](auto entity) {
					auto [change, atts] = view.get<const ChangeAttribute, AttributeSet>(entity);

					for (auto& i : change)
					{
						switch (i.first.value())
						{
						case Database::AHealth.value():
						{
							auto& value = ECS.get<HealthComponent>(entity);
							ChangeValueKeepPercent(*factory, i.first, atts, value.curValue,
								value.maxValue, i.second);
							break;
						}
						case Database::AMana.value():
						{
							auto& value = ECS.get<ManaComponent>(entity);
							ChangeValueKeepPercent(*factory, i.first, atts, value.curValue,
								value.maxValue, i.second);
							break;
						}
						default:
							GetAttributeByName(*factory, atts, i.first) += i.second;
							break;
						}
					}
					});
			}
			
			// Health
			{
				auto view = ECS.view<const ChangeHealth, HealthComponent>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &ECS, &factory](auto entity) {

					auto [amount, value] = view.get<const ChangeHealth, HealthComponent>(entity);

					value.curValue = std::min(unsigned int(value.curValue + amount), value.maxValue);
					});
				ECS.view<ChangeHealth, HealthComponent, OnDeathListener>().each([&ECS](auto entity, const auto&, const auto& health, const OnDeathListener& listener) {
					if (health.curValue == 0)
						for (auto& f : listener)
							if (f) f(entity, ECS);
					});
				ECS.clear<ChangeHealth>();
			}
			// Mana
			{
				auto view = ECS.view<const ChangeMana, ManaComponent>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &ECS, &factory](auto entity) {

					auto [amount, value] = view.get<const ChangeMana, ManaComponent>(entity);

					value.curValue = std::min(unsigned int(value.curValue + amount), value.maxValue);
				});
				ECS.clear<ChangeMana>();
			}
			
		}
	}
private:
	//Use for health or Mana only
	static void ChangeValueKeepPercent(const AttributeFactory& factory, entt::hashed_string attName,
		AttributeSet& atts, unsigned int& curValue, unsigned int& maxValue, Attribute amount)
	{
		const auto index = GetIndex(factory, attName);
		maxValue = GetMaxValueByIndex(atts, index);
		float percent = (float)curValue / (float)maxValue;
		auto& att = GetAttributeByIndex(atts, index);
		att += amount;

		curValue = unsigned int(percent * (float)att.getFinalValue());
	}
	static size_t GetIndex(const AttributeFactory& factory, entt::hashed_string attName)
	{
		return factory.GetIndex(attName);
	}

	static int GetMaxValueByName(const AttributeFactory& factory, const AttributeSet& atts, entt::hashed_string attName)
	{
		return atts[factory.GetIndex(attName)].getFinalValue();
	}
	static int GetMaxValueByIndex(const AttributeSet& atts, size_t index)
	{
		return atts[index].getFinalValue();
	}

	static Attribute& GetAttributeByName(const AttributeFactory& factory, AttributeSet& atts, entt::hashed_string attName)
	{
		return atts[factory.GetIndex(attName)];
	}
	static Attribute& GetAttributeByIndex(AttributeSet& atts, size_t index)
	{
		return atts[index];
	}
};