#pragma once
#include "entt/entt.hpp"
#include "Box2D/Box2D.h"
#include "Codex.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gird.h"
#include <random>
struct Locator
{
	using Graphic = entt::service_locator<Graphics>;
	using ECS = entt::service_locator<entt::registry>;
	using Random = entt::service_locator<std::mt19937>;
	using Mouse = entt::service_locator<Mouse>;
	using Keyboard = entt::service_locator<Keyboard>;
};