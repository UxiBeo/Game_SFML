#pragma once
#include <vector>
#include <algorithm>
#include <execution>
#include <optional>
#include "entt/core/hashed_string.hpp"

/*---------------Attribute Factory------------------------*/
class AttributeFactory
{
	using AttributeType = entt::hashed_string::hash_type;
	struct AttributeMap
	{
		AttributeFactory::AttributeType name;
		bool has_dependent = false;
		using indexAndName = std::pair<unsigned char, AttributeFactory::AttributeType>;
		std::vector<indexAndName> dependentAtts;
	};
public:
	template<typename T>
	std::vector<T> MakeAttribute() const
	{
		std::vector<T> vec(lookUpMap.size());
		return std::move(vec);
	}
	const AttributeMap& GetMap(int index) const
	{
		if (index < lookUpMap.size())
		{
			return lookUpMap[index];
		}
		assert(false && "Can't find attribute");
		return AttributeMap();
	}
	const int GetIndex(entt::hashed_string attName) const
	{
		if (auto i = binary_find(lookUpMap, attName.value()))
			return *i;

		assert(false && "Can't find attribute");
		return -1;
	}
	// add attribute to build look up map
	void Attribute(entt::hashed_string attribute)
	{
		lookUpMap.emplace_back();
		lookUpMap.back().name = attribute.value();
	}
	// add dependent attribute to build look up map
	void AddDependentAttribute(entt::hashed_string attrName, std::vector<entt::hashed_string> dependent)
	{
		lookUpMap.emplace_back();
		lookUpMap.back().name = attrName.value();

		dependentTemp.emplace_back();
		auto& temp = dependentTemp.back();
		temp.first = attrName.value();
		for (auto& i : dependent)
		{
			temp.second.push_back(i.value());
		}
	}
	//bake look up map for future use
	void CookAttributeMap()
	{
		SortRemoveDuplicate();
		BuildDependTemp();
	}
	
private:
	void SortRemoveDuplicate()
	{
		std::sort(std::execution::par, lookUpMap.begin(), lookUpMap.end(),
			[](const AttributeMap& lhs, const AttributeMap& rhs) { return lhs.name < rhs.name; });

		lookUpMap.erase(
			std::unique(std::execution::par, lookUpMap.begin(), lookUpMap.end(),
				[](const AttributeMap& lhs, const AttributeMap& rhs) { return lhs.name == rhs.name; }),
			lookUpMap.end());
		lookUpMap.shrink_to_fit();

		std::sort(std::execution::par, dependentTemp.begin(), dependentTemp.end(),
			[](const pairTemp& lhs, const pairTemp& rhs) { return lhs.first < rhs.first; });

		dependentTemp.erase(
			std::unique(std::execution::par, dependentTemp.begin(), dependentTemp.end(),
				[](const pairTemp& lhs, const pairTemp& rhs) { return lhs.first == rhs.first; }),
			dependentTemp.end());
	}
	void BuildDependTemp()
	{
		std::for_each(std::execution::par, dependentTemp.begin(), dependentTemp.end(), [this](const pairTemp& pair) {
			if (auto i = binary_find(lookUpMap, pair.first))
			{
				auto& attMap = lookUpMap[*i];
				attMap.has_dependent = true;
				for (auto att : pair.second)
				{
					if (auto it = binary_find(lookUpMap, att))
					{
						attMap.dependentAtts.emplace_back(std::make_pair(unsigned char(*it), att));
					}
					else
					{
						assert(false && "Can't find dependent attribute in lookup map");
					}
				}
			}
			else
			{
				assert(false && "Can't find attribute in lookup map");
			}
		});
		dependentTemp.resize(0);
	}

	template<typename Container>
	const static std::optional<size_t> binary_find(const Container& vec, AttributeType attrName)
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), attrName,
			[](const Container::value_type& lhs, const AttributeType& attrName) { return lhs.name < attrName; });
		return it != vec.end() && *it == attrName ? std::distance(vec.begin(), it) : std::optional<size_t>{};
	}
private:
	std::vector<AttributeMap> lookUpMap;
	using pairTemp = std::pair<AttributeType, std::vector<AttributeType>>;
	std::vector<pairTemp> dependentTemp;
};
/*---------------Attribute Factory------------------------*/