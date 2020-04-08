#pragma once
#include <set>
#include <optional>
#include <execution>
#include "entt/core/hashed_string.hpp"
class Tag
{
public:
	using Bitfiled = unsigned int;
	void AddTag(entt::hashed_string tag)
	{
		tagContainer.insert(tag.value());
	}
	void AddTag(const Tag& another)
	{
		for (auto& t : another.GetTagContainer())
		{
			tagContainer.insert(t);
		}
	}
	
	Bitfiled GetBitField(const entt::hashed_string& tag) const
	{
		if (auto it = tagContainer.find(tag.value()); it != tagContainer.end())
		{
			return 1 << (std::distance(tagContainer.begin(), it));
		}
		else
		{
			assert(false && "Can't find tag");
			return 0;
		}
	}
	Bitfiled GetBitField(std::vector<entt::hashed_string>& tags)
	{
		unsigned int bitfiled = 0;
		for (const auto& t : tags)
		{
			bitfiled |= GetBitField(t);
		}
		return bitfiled;
	}
	
	const std::set<entt::hashed_string::hash_type>& GetTagContainer() const
	{
		return tagContainer;
	}
private:
	std::set<entt::hashed_string::hash_type> tagContainer;
};