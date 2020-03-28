#pragma once
#include "../HashStringDataBase.h"
#include "entt/signal/delegate.hpp"
#include <numeric>
#include <algorithm>
#include <vector>
class Attribute
{
public:
	Attribute(int value = 0, float multiplier = 0.0f, int minValue = std::numeric_limits<int>::min())
		:
		value(value),
		multiplier(multiplier),
		minValue(minValue)
	{}
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

using ChangeAttribute = std::vector<std::pair<entt::hashed_string, Attribute>>;
using AttributeSet = std::vector<Attribute>;
using ChangeHealth = int;
using ChangeMana = int;
struct HealthComponent
{
	unsigned int curValue;
	unsigned int maxValue;
};
struct ManaComponent
{
	unsigned int curValue;
	unsigned int maxValue;
};
using OnDeathNotify = std::vector<entt::delegate<void(entt::entity, entt::registry&)>>;