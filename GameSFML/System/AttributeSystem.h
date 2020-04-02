#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AttributeFactory.h"
#include "../Component/AttributeComponent.h"
class AttributeSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final;
public:
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