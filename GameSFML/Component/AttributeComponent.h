#pragma once
#include "../HashStringDataBase.h"
#include "entt/signal/delegate.hpp"
#include <numeric>
#include <algorithm>
#include <vector>
namespace STATS
{
	class Value
	{
	public:
		Value(float baseValue = 0, float baseMultiplier = 0.0f, float value = 0, float multiplier = 0.0f,
			float minValue = std::numeric_limits<float>::min())
			:
			baseValue(baseValue),
			baseMultiplier(baseMultiplier),
			value(value),
			multiplier(multiplier),
			minValue(minValue)
		{}
		Value operator+(const Value& rhs) const
		{
			return Value(baseValue + rhs.baseValue, baseMultiplier + rhs.baseMultiplier, 
				value + rhs.value, multiplier + rhs.multiplier);
		}
		Value& operator+=(const Value& rhs)
		{
			return *this = *this + rhs;
		}
		Value operator-(const Value& rhs) const
		{
			return Value(baseValue - rhs.baseValue, baseMultiplier - rhs.baseMultiplier, 
				value - rhs.value, multiplier - rhs.multiplier);
		}
		Value& operator-=(const Value& rhs)
		{
			return *this = *this - rhs;
		}

		float getFinalValue() const
		{
			return std::max(minValue, baseValue * (1.0f + baseMultiplier) * (1.0f + multiplier) + value);
		}
	private:
		float baseValue = 0;
		float baseMultiplier = 0.0f;
		float value = 0;
		float multiplier = 0.0f;
		float minValue = std::numeric_limits<float>::min();
	};

	using Base = std::vector<Value>;
	using ChangeBase = std::vector<std::pair<size_t, Value>>;

	using Depent = std::vector<Value>;
	using ChangeDepent= std::vector<std::pair<size_t, Value>>;

	struct Special
	{
		size_t indexSet;
		int curValue;
		unsigned int maxValue;
		bool trackReachZero;
	};
	using ChangeSpecial = std::vector<std::pair<size_t, float>>;
	using SpecialSet = std::vector<Special>;
}
