#pragma once
#include "nlohmann/json.hpp"
#include "entt/core/hashed_string.hpp"
#include <fstream>
#include <sstream>
#include <optional>
namespace MaxxUtility
{
	static const nlohmann::json GetJson(entt::hashed_string filename)
	{
		std::ifstream input(static_cast<const char*>(filename));
		nlohmann::json Json;
		input >> Json;
		input.close();
		return Json;
	}
	//template <class Container>
	//std::optional<size_t> binary_find(const Container& cont, const entt::hashed_string value)
	//{
	//	const auto i = std::lower_bound(cont.begin(), cont.end(), value);
	//	return i != cont.end() && *i == value ? std::distance(cont.begin(), i) : std::optional<size_t>{};
	//}
}