#pragma once
#include <numeric>
#include <algorithm>
#include <array>
namespace RPGS
{
	struct CurrentConsume
	{
		int data;
	};
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
		Value& operator+=(const Value& rhs)
		{
			return *this = *this + rhs;
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
			return *this = *this - rhs;
		}
		float GetValueEfficient(float first, float second, float thirst)
		{
			return 0.0f;
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

		ConsumeValue operator+(const ConsumeValue& rhs) const
		{
			ConsumeValue newValue = *this + rhs.maxValue;
			newValue.curValue += rhs.curValue;
			newValue.curMaxValue += rhs.curMaxValue;
			return newValue;
		}
		ConsumeValue& operator+=(const ConsumeValue& rhs)
		{
			return *this = *this + rhs;
		}
		ConsumeValue operator-(const ConsumeValue& rhs) const
		{
			ConsumeValue newValue = *this - rhs.maxValue;
			newValue.curValue -= rhs.curValue;
			newValue.curMaxValue -= rhs.curMaxValue;
			return newValue;
		}
		ConsumeValue& operator-=(const ConsumeValue& rhs)
		{
			return *this = *this - rhs;
		}

		ConsumeValue operator+(const CurrentConsume& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.curValue += rhs.data;
			return newValue;
		}
		ConsumeValue& operator+=(const CurrentConsume& rhs)
		{
			this->curValue += rhs.data;
			return *this;
		}
		ConsumeValue operator-(const CurrentConsume& rhs) const
		{
			ConsumeValue newValue = *this;
			newValue.curValue -= rhs.data;
			return newValue;
		}
		ConsumeValue& operator-=(const CurrentConsume& rhs)
		{
			this->curValue -= rhs.data;
			return *this;
		}
		Value maxValue;
		int curValue;
		float curMaxValue;
	};
	
	enum ENormal
	{
		Strength,
		Constitution,
		Dexterity,
		Intelligence,
	};
	enum EDependent
	{
		Attack,
		Defence,
		CirtChange,
		CritDame
	};
	enum EConsume
	{
		HealthPoint,
		ManaPoint,
	};
	template <typename EType, typename ValueType>
	struct SBase
	{
		std::array<ValueType, sizeof(EType) + 1> data;
	};
	template <typename EType, typename ModifiedType>
	struct SModified
	{
		std::array<ModifiedType, sizeof(EType) + 1> data;
	};
	template <typename EType, typename ModifiedType>
	struct ComingDame
	{
		std::array<ModifiedType, sizeof(EType) + 1> data;
	};
	

}


/*
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
		float GetValueEfficient(float first, float second, float thirst)
		{
			return 0.0f;
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
	class Map
	{
	public:
		struct Depend
		{
			struct ByIndex
			{
				char index = 0;
				float perUnit = 0.0f;
				float percent = 0.0f;
			};
			struct ByName
			{
				entt::hashed_string name;
				float perUnit = 0.0f;
				float percent = 0.0f;
			};
			std::vector<ByIndex> dependentAtts;
			
		};
	public:
		std::pair<char, bool> GetIndex(entt::hashed_string attName) const
		{
			if (auto it = baseMap.find(attName.value()); it != baseMap.end())
			{
				return std::make_pair((char)std::distance(baseMap.begin(), it), true);
			}
			if (auto it = consumeMap.find(attName.value()); it != consumeMap.end())
			{
				return std::make_pair((char)std::distance(consumeMap.begin(), it), false);
			}
			assert(false && "Can't find attribute");
			return {0, false};
		}
		char GetHealthIndex() const
		{
			return HealthIndex;
		}
		void BuildMap(entt::hashed_string filename)
		{
			const nlohmann::json Json = MaxxUtility::GetJson(filename);

			for (auto& n : Json["BaseType"])
			{
				baseMap.insert(entt::hashed_string(n["name"].get<std::string>().c_str()));
			}
			entt::hashed_string healthName;
			for (auto& n : Json["ConsumeType"])
			{
				consumeMap.insert(entt::hashed_string(n["name"].get<std::string>().c_str()));
				if (n["main"].get<bool>() == true)
				{
					healthName = entt::hashed_string(n["name"].get<std::string>().c_str());
				}
			}
			HealthIndex = GetIndex(healthName);
		}
	private:
		std::set<entt::hashed_string::hash_type> baseMap;
		std::set<entt::hashed_string::hash_type> consumeMap;
		char HealthIndex = 0;
	};

	struct Basic
	{
		std::vector<Value> data;
	};
	struct ChangeBasic
	{
		std::map<char, Value> data;
	};
	struct Consume
	{
		std::vector<Value> data;
		std::vector<int> curValue;
		std::vector<float> maxValue;
	};
	struct ChangeBaseConsume
	{
		std::map<char, Value> data;
	};
	struct ChangeConsume
	{
		std::map<char, float> data;
	};
	struct ReachZero{};
}
*/