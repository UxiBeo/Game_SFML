#pragma once
#include <numeric>
#include <array>
#define LEAF_ELEMENT_TYPES \
	X(STRENGTH) \
	X(CONSTITUTION) \
	X(DEXTERITY) \
	X(INTELLIGENCE) \
	X(CRITCHANGE) \
	X(CRITDAME) \
	X(ATTACK)\
	X(DEFENCE)

namespace RPGS
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
		bool isZero() const
		{
			return baseValue == 0.0f && baseMultiplier == 0.0f && value == 0.0f && multiplier == 0.0f;
		}
	public:
		float baseValue = 0;
		float baseMultiplier = 0.0f;
		float value = 0;
		float multiplier = 0.0f;
		float minValue = std::numeric_limits<float>::min();
	};
	struct ConsumeValue
	{
		ConsumeValue operator+(const Value& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.maxValue += rhs;
			float percent = (float)newValue.curValue / newValue.curMaxValue;
			newValue.curMaxValue = newValue.maxValue.getFinalValue();
			newValue.curValue = int(percent * curMaxValue);
			return newValue;
		}
		ConsumeValue& operator+=(const Value& rhs)
		{
			this->maxValue += rhs;
			return *this;
		}
		ConsumeValue operator-(const Value& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.maxValue -= rhs;
			float percent = (float)newValue.curValue / newValue.curMaxValue;
			newValue.curMaxValue = newValue.maxValue.getFinalValue();
			newValue.curValue = int(percent * curMaxValue);
			return newValue;
		}
		ConsumeValue& operator-=(const Value& rhs)
		{
			this->maxValue -= rhs;
			return *this;
		}
		ConsumeValue operator+(const int& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.curValue += rhs;
			return newValue;
		}
		ConsumeValue& operator+=(const int& rhs)
		{
			this->curValue += rhs;
			return *this;
		}
		ConsumeValue operator-(const int& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.curValue -= rhs;
			return newValue;
		}
		ConsumeValue& operator-=(const int& rhs)
		{
			this->curValue -= rhs;
			return *this;
		}
		Value maxValue;
		int curValue;
		float curMaxValue;
	};

	enum AttributeType
	{
		#define X(el) el,
		LEAF_ELEMENT_TYPES
		#undef X
	};
	template <AttributeType T>
	struct Attribute
	{
		Value value;
	};
	template <AttributeType T>
	struct ModifiedAttribute
	{
		Value value;
	};
	
}

#ifndef AttXMacro
#undef LEAF_ELEMENT_TYPES
#endif