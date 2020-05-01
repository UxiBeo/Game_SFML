#pragma once
#include "entt/entt.hpp"

struct ImguiComponent
{
	entt::delegate<void(entt::registry&)> mrD;
};

struct ShowImgui {};