#pragma once
#include "nlohmann/json.hpp"
#include "entt/core/hashed_string.hpp"
#include <fstream>
namespace MaxU
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

    constexpr float PI_F = 3.14159265358979323846f;
    template <typename T>
    constexpr auto sq(const T& x) noexcept
    {
        return x * x;
    }

    template<typename T>
    constexpr T gauss(T x, T sigma) noexcept
    {
        const auto ss = sq(sigma);
        return ((T)1.0 / sqrt((T)2.0 * (T)PI_F * ss)) * exp(-sq(x) / ((T)2.0 * ss));
    }
}