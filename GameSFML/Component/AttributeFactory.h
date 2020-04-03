#pragma once
#include <vector>
#include <algorithm>
#include <execution>
#include <optional>
#include "entt/core/hashed_string.hpp"
#include "../MaxxUtility.h"

class AttributeFactory
{
public:
	using AttributeType = entt::hashed_string::hash_type;
	struct DependIndex
	{
		size_t index = 0;
		float perUnit = 0.0f;
		float percent = 0.0f;
	};
	struct DependName
	{
		entt::hashed_string name;
		float perUnit = 0.0f;
		float percent = 0.0f;
	};
	struct DependentAtt
	{
		AttributeFactory::AttributeType name;
		std::vector<DependIndex> dependentAtts;
	};
public:
	AttributeFactory()
	{
		const nlohmann::json Json = MaxxUtility::GetJson(Database::AttributeStat);

		for (auto& n : Json["BaseType"])
		{
			AddBase(entt::hashed_string(n["name"].get<std::string>().c_str()));
		}
		SortBase();
		for (auto& n : Json["DependentType"])
		{
			auto attName = entt::hashed_string(n["name"].get<std::string>().c_str());
			std::vector<DependName> dependAtts;
			for (auto& d : n["dependent"])
			{
				dependAtts.emplace_back();
				auto& temp = dependAtts.back();
				temp.name = entt::hashed_string(d["name"].get<std::string>().c_str());
				temp.perUnit = d["perPoint"].get<float>();
				temp.percent = d["percen"].get<float>();

			}
			AddDependent(attName, dependAtts);
		}
		SortDependent();
	}
	template<typename T>
	std::vector<T> MakeBaseAttribute() const
	{
		std::vector<T> vec(base.size());
		return std::move(vec);
	}
	size_t GetIndex(entt::hashed_string attName) const
	{
		if (auto i = binary_find_base(base, attName.value()))
			return *i;

		if (auto i = binary_find_dependent(dependent, attName.value()))
			return *i;

		assert(false && "Can't find attribute");
		return 0;
	}
	
private:
	void AddBase(entt::hashed_string name)
	{
		base.emplace_back(name.value());
	}
	void AddDependent(entt::hashed_string name, std::vector<DependName> dName)
	{
		dependent.emplace_back();
		auto& att = dependent.back();
		att.name = name.value();
		for (auto& d : dName)
		{
			att.dependentAtts.emplace_back();
			auto& temp = att.dependentAtts.back();
			temp.index = GetIndex(d.name);
			temp.perUnit = d.perUnit;
			temp.percent = d.percent;
		}
	}
	void SortBase()
	{
		std::sort(std::execution::par_unseq, base.begin(), base.end());
	}
	void SortDependent()
	{
		std::sort(std::execution::par_unseq, dependent.begin(), dependent.end(), [](auto& lhs, auto& rhs) { return lhs.name < rhs.name; });
	}
	std::optional<size_t> binary_find_base(const std::vector<entt::hashed_string::hash_type>& vec, AttributeType attrName) const
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), attrName,
			[](const entt::hashed_string::hash_type& lhs, const AttributeType& attrName) { return lhs < attrName; });
		return it != vec.end() && *it == attrName ? std::distance(vec.begin(), it) : std::optional<size_t>{};
	}
	std::optional<size_t> binary_find_dependent(const std::vector<DependentAtt>& vec, AttributeType attrName) const
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), attrName,
			[](const DependentAtt& lhs, const AttributeType& attrName) { return lhs.name < attrName; });
		return it != vec.end() && it->name == attrName ? std::distance(vec.begin(), it) : std::optional<size_t>{};
	}
private:
	std::vector<entt::hashed_string::hash_type> base;
	std::vector<DependentAtt> dependent;
};