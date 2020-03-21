#pragma once
#include <vector>
#include <execution>
#include "entt/core/hashed_string.hpp"
class GameplayTag
{
	using tagType = entt::hashed_string::hash_type;
public:
	bool AddTag(entt::hashed_string tag)
	{
		if (!HasTag(tag))
		{
			tagContainer.emplace_back(tag.value());
			SortTags();
			return true;
		}
		return false;
	}
	bool AddTag(const GameplayTag& another)
	{
		const auto& anotherCon = another.GetTagContainer();
		std::vector<tagType> dst;
		std::set_union(tagContainer.begin(), tagContainer.end(), anotherCon.begin(), anotherCon.end(), std::back_inserter(dst));
		if (dst.size() > tagContainer.size())
		{
			tagContainer = std::move(dst);
			return true;
		}
		return false;
	}
	bool RemoveTag(entt::hashed_string tag)
	{
		if (const auto it = binary_find(GetTagContainer(), tag.value()))
		{
			std::swap(tagContainer[*it], tagContainer.back());
			tagContainer.pop_back();
			SortTags();
			return true;
		}
		return false;
	}
	bool RemoveTag(const GameplayTag& another)
	{
		const auto& anotherCon = another.GetTagContainer();
		std::vector<tagType> diff;
		std::set_difference(tagContainer.begin(), tagContainer.end(),
			anotherCon.begin(), anotherCon.end(), std::back_inserter(diff));
		if (diff.size() < tagContainer.size())
		{
			tagContainer = std::move(diff);
			return true;
		}
		return false;
	}
	const bool HasTag(const GameplayTag& another) const
	{
		const auto& anotherCon = another.GetTagContainer();
		return std::includes(std::execution::par, tagContainer.begin(), tagContainer.end(), anotherCon.begin(), anotherCon.end());
	}
	const bool HasTag(entt::hashed_string tag) const
	{
		return HasTagType(GetTagContainer(),tag.value());
	}
	const bool HasAny(const GameplayTag& another) const
	{
		const auto& anotherCon = another.GetTagContainer();
		return std::any_of(std::execution::par,anotherCon.begin(), anotherCon.end(), [this](const tagType& tag) { return HasTagType(tagContainer,tag); });
	}
	bool HasAnyAndRemove(const GameplayTag& another)
	{
		return RemoveTag(another);
	}
	const std::vector<tagType>& GetTagContainer() const
	{
		return tagContainer;
	}
private:
	void SortTags() 
	{
		std::sort(std::execution::par, tagContainer.begin(), tagContainer.end());
	}
	const static bool HasTagType(const std::vector<tagType>& vec, tagType tag)
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), tag,
			[](const tagType& lhs, const tagType& tag) { return lhs < tag; });
		return it != vec.end() && *it == tag ? true : false;
	}
	const static std::optional<size_t> binary_find(const std::vector<tagType>& vec, tagType tag)
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), tag,
			[](const tagType& lhs, const tagType& tag) { return lhs < tag; });
		return it != vec.end() && *it == tag ? std::distance(vec.begin(), it) : std::optional<size_t>{};
	}
private:
	std::vector<tagType> tagContainer;
	 bool is_sorted = false;
};