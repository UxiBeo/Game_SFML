#pragma once
#include <numeric>

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
			float percent = newValue.curValue / newValue.curMaxValue;
			newValue.curMaxValue = newValue.maxValue.getFinalValue();
			newValue.curValue = percent * curMaxValue;
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
			float percent = newValue.curValue / newValue.curMaxValue;
			newValue.curMaxValue = newValue.maxValue.getFinalValue();
			newValue.curValue = percent * curMaxValue;
			return newValue;
		}
		ConsumeValue& operator-=(const Value& rhs)
		{
			this->maxValue -= rhs;
			return *this;
		}
		ConsumeValue operator+(const float& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.curValue += rhs;
			return newValue;
		}
		ConsumeValue& operator+=(const float& rhs)
		{
			this->curValue += rhs;
			return *this;
		}
		ConsumeValue operator-(const float& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.curValue -= rhs;
			return newValue;
		}
		ConsumeValue& operator-=(const float& rhs)
		{
			this->curValue -= rhs;
			return *this;
		}
		Value maxValue;
		float curValue;
		float curMaxValue;
	};
	
	enum AttributeType
	{
		CritChange = 1 << 1,
		CritDame = 1 << 2,
		Attack = 1 << 3,
		Defence = 1 << 4,
		HealthPoint = 1 << 5,
		ManaPoint = 1 << 6
	};
	struct AttributePack
	{
		unsigned int value;
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

	template <AttributeType T>
	struct ModifiedConsumeCur
	{
		float value;
	};
	template <AttributeType T>
	struct InputDame
	{
		float value;
	};
	/*-----------Template Specialization------------*/
	template <>
	struct Attribute<AttributeType::HealthPoint>
	{
		ConsumeValue value;
	};
	template <>
	struct Attribute<AttributeType::ManaPoint>
	{
		ConsumeValue value;
	};
}