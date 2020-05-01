#pragma once
#include <numeric>
namespace GES
{
	struct Value
	{
	public:
		Value operator+(const Value& rhs) const
		{
			Value newValue = *this;
			newValue.baseValue += rhs.baseValue;
			newValue.baseMultiplier += rhs.baseMultiplier;
			newValue.value += rhs.value;
			newValue.multiplier += rhs.multiplier;
			return newValue;
		}
		Value operator*(const float& rhs) const
		{
			Value newValue = *this;
			newValue.baseValue *= rhs;
			newValue.baseMultiplier *= rhs;
			newValue.value *= rhs;
			newValue.multiplier *= rhs;
			return newValue;
		}
		Value& operator+=(const Value& rhs)
		{
			baseValue += rhs.baseValue;
			baseMultiplier += rhs.baseMultiplier;
			value += rhs.value;
			multiplier += rhs.multiplier;
			return *this;
		}
		Value operator-(const Value& rhs) const
		{
			Value newValue = *this;
			newValue.baseValue -= rhs.baseValue;
			newValue.baseMultiplier -= rhs.baseMultiplier;
			newValue.value -= rhs.value;
			newValue.multiplier -= rhs.multiplier;
			return newValue;
		}
		Value& operator-=(const Value& rhs)
		{
			baseValue -= rhs.baseValue;
			baseMultiplier -= rhs.baseMultiplier;
			value -= rhs.value;
			multiplier -= rhs.multiplier;
			return *this;
		}
		
		float getFinalValue() const
		{
			return std::max(minValue, baseValue * (1.0f + baseMultiplier) * (1.0f + multiplier) + value);
		}
	public:
		float baseValue = 0;
		float baseMultiplier = 0.0f;
		float value = 0;
		float multiplier = 0.0f;
		float minValue = std::numeric_limits<float>::min();
	};
	struct SpecialValue
	{
		SpecialValue operator+(const Value& rhs) const
		{
			SpecialValue newValue = *this;
			newValue.maxValue += rhs;
			float percent = newValue.curValue / newValue.curMaxValue;
			newValue.curMaxValue = newValue.maxValue.getFinalValue();
			newValue.curValue = percent * curMaxValue;
			return newValue;
		}
		SpecialValue& operator+=(const Value& rhs)
		{
			this->maxValue += rhs;
			return *this;
		}
		SpecialValue operator-(const Value& rhs) const
		{
			SpecialValue newValue = *this;
			newValue.maxValue -= rhs;
			float percent = newValue.curValue / newValue.curMaxValue;
			newValue.curMaxValue = newValue.maxValue.getFinalValue();
			newValue.curValue = percent * curMaxValue;
			return newValue;
		}
		SpecialValue& operator-=(const Value& rhs)
		{
			this->maxValue -= rhs;
			return *this;
		}
		SpecialValue operator+(const float& rhs) const
		{
			SpecialValue newValue = *this;
			newValue.curValue += rhs;
			return newValue;
		}
		SpecialValue& operator+=(const float& rhs)
		{
			this->curValue += rhs;
			return *this;
		}
		SpecialValue operator-(const float& rhs) const
		{
			SpecialValue newValue = *this;
			newValue.curValue -= rhs;
			return newValue;
		}
		SpecialValue& operator-=(const float& rhs)
		{
			this->curValue -= rhs;
			return *this;
		}
		Value maxValue;
		float curValue;
		float reserve = 0.0f;
		float curMaxValue;
	};
	

	enum AttributeType
	{
		CritChange,
		CritDame,
		Attack,
		Defence,
		HealthPoint,
		ManaPoint,
		Count
	};
	struct AttributePack
	{
		unsigned int bitmask = 0;
		std::array<entt::entity, AttributeType::Count> attribute{entt::null};
	};
	struct InputDame
	{
		float value = 0.0f;
	};
}