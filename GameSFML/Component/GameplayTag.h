#pragma once
#include <vector>
#include <optional>
#include <execution>
#include "entt/core/hashed_string.hpp"
class Tag
{
public:
	using Bitfiled = unsigned int;
public:
	void AddTag(entt::hashed_string tag)
	{
		tagContainer.emplace_back(tag);
		is_sorted = false;
	}
	void AddTag(const Tag& another)
	{
		const auto& anotherCon = another.GetTagContainer();
		tagContainer.insert(tagContainer.end(), anotherCon.begin(), anotherCon.end());
		is_sorted = false;
	}
	
	Bitfiled GetBitField(const entt::hashed_string& tag) const
	{
		if (const auto it = binary_find(GetTagContainer(), tag))
		{
			return 0 << (*it + 1);
		}
		else
		{
			assert(false && "Can't find tag");
			return 0;
		}
	}
	Bitfiled GetBitField(std::vector<entt::hashed_string>& tags)
	{
		Bitfiled bitfiled = 0;
		auto pred = [&bitfiled,this](const entt::hashed_string& tags) {
			bitfiled |= this->GetBitField(tags);
		};
		std::for_each(tags.begin(), tags.end(), pred);
		return bitfiled;
	}
	
	const std::vector<entt::hashed_string>& GetTagContainer() const
	{
		return tagContainer;
	}
	void SortAndRemoveDuplicate()
	{
		std::sort(std::execution::par, tagContainer.begin(), tagContainer.end());
		auto last = std::unique(tagContainer.begin(), tagContainer.end());
		tagContainer.erase(last, tagContainer.end());
		is_sorted = true;
	}
private:
	
	const static std::optional<size_t> binary_find(const std::vector<entt::hashed_string>& vec, entt::hashed_string tag)
	{
		const auto it = std::lower_bound(vec.begin(), vec.end(), tag,
			[](const entt::hashed_string& lhs, const entt::hashed_string& tag) { return lhs.value() < tag.value(); });
		return it != vec.end() && it->value() == tag.value() ? std::distance(vec.begin(), it) : std::optional<size_t>{};
	}

	/*const bool HasTag(const GameplayTag& another) const
	{
		const auto& anotherCon = another.GetTagContainer();
		return std::includes(std::execution::par, tagContainer.begin(), tagContainer.end(),
			anotherCon.begin(), anotherCon.end());
	}
	const bool HasTag(entt::hashed_string tag) const
	{
		if (const auto it = binary_find(GetTagContainer(), tag))
		{
			return true;
		}
		return false;
	}
	const bool HasAny(const GameplayTag& another) const
	{
		const auto& anotherCon = another.GetTagContainer();
		return std::any_of(std::execution::par, anotherCon.begin(), anotherCon.end(), [this](const entt::hashed_string& tag) { return this->HasTag(tag); });
	}
	bool RemoveTag(entt::hashed_string tag)
	{
		if (const auto it = binary_find(GetTagContainer(), tag))
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
		std::vector<entt::hashed_string> diff;
		std::set_difference(tagContainer.begin(), tagContainer.end(),
			anotherCon.begin(), anotherCon.end(), std::back_inserter(diff));
		if (diff.size() < tagContainer.size())
		{
			tagContainer = std::move(diff);
			return true;
		}
		return false;
	}
	bool HasAnyAndRemove(const GameplayTag& another)
	{
		return RemoveTag(another);
	}*/
private:
	std::vector<entt::hashed_string> tagContainer;
	bool is_sorted = false;
};