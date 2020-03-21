#include "AttributeFactory.h"
#include "HashStringDataBase.h"
#include <numeric>
#include <algorithm>
class Attribute
{
public:
	Attribute(int value = 0, float multiplier = 0.0f, int minValue = std::numeric_limits<int>::min())
		:
		value(value),
		multiplier(multiplier),
		minValue(minValue)
	{}
	Attribute() = default;
	Attribute operator+(const Attribute& rhs) const
	{
		return Attribute(value + rhs.value, multiplier + rhs.multiplier);
	}
	Attribute& operator+=(const Attribute& rhs)
	{
		return *this = *this + rhs;
	}
	Attribute operator-(const Attribute& rhs) const
	{
		return Attribute(value - rhs.value, multiplier - rhs.multiplier);
	}
	Attribute& operator-=(const Attribute& rhs)
	{
		return *this = *this - rhs;
	}

	int getFinalValue() const
	{
		return std::max(minValue,int((float)value * (1.0f + multiplier)));
	}
private:
	int value = 0;
	float multiplier = 0;
	int minValue = std::numeric_limits<int>::min();
};

class MyGameAttributes
{
public:
	MyGameAttributes(const AttributeFactory& factory)
		:
		factory(factory),
		atts(std::move(factory.MakeAttribute<Attribute>()))
	{}
	void ChangeValue(entt::hashed_string attName, Attribute amount)
	{
		switch (attName.value())
		{
		case Database::AHealth.value():
			ChangeValueKeepPercent(attName, curHealth, amount);
			break;
		case Database::AMana.value():
			ChangeValueKeepPercent(attName, curHealth, amount);
			break;
		default:
			ChangeValue(attName, amount);
			break;
		}
	}
	//Use for health or Mana only
	void ChangeCurrentValue(entt::hashed_string attName, int amount)
	{
		if (attName == Database::AHealth)
		{
			std::clamp(curHealth += amount, 0, GetMaxValueByName(attName));
		}
		else
		{
			assert(attName == Database::AMana);
			std::clamp(curHealth += amount, 0, GetMaxValueByName(attName));
			// mana for sure
		}
	}
private:
	void ChangeValueKeepPercent(entt::hashed_string attName, int& curValue, Attribute amount)
	{
		const auto index = GetIndex(attName);

		float percent = (float)curValue / (float)GetMaxValueByIndex(index);
		auto& att = GetAttributeByIndex(index);
		att += amount;

		curValue = int(percent * (float)att.getFinalValue());
	}
	void ChangeValue(entt::hashed_string attName, Attribute amount)
	{
		GetAttributeByName(attName) += amount;
	}
private:
	size_t GetIndex(entt::hashed_string attName) const
	{
		return factory.GetIndex(attName);
	}

	int GetMaxValueByName(entt::hashed_string attName) const
	{
		return atts[factory.GetIndex(attName)].getFinalValue();
	}
	int GetMaxValueByIndex(size_t index) const
	{
		return atts[index].getFinalValue();
	}

	Attribute& GetAttributeByName(entt::hashed_string attName)
	{
		return atts[factory.GetIndex(attName)];
	}
	Attribute& GetAttributeByIndex(size_t index)
	{
		return atts[index];
	}
private:
	int curHealth;
	int curMana;
	std::vector<Attribute> atts;
	const AttributeFactory& factory;
};