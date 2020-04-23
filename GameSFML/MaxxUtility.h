#pragma once
#include "nlohmann/json.hpp"
#include "entt/core/hashed_string.hpp"
#include <fstream>
namespace MaxxUtility
{
	static const nlohmann::json GetJson(entt::hashed_string filename)
	{
		std::ifstream input(static_cast<const char*>(filename));
		if (!input.is_open()) {
			assert(false && "failed to open ");
		}
		nlohmann::json Json;
		input >> Json;
		input.close();
		return Json;
	}
}