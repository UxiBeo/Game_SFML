#pragma once
#include "box2d/box2d.h"
#include "SFML/System/Vector2.hpp"
struct TargetPosition
{
	b2Vec2 goalPos{0.0f,0.0f};
	b2Vec2 curPos{ 0.0f,0.0f };
	sf::Vector2i mousePos{ 0,0 };
};

struct PlayerController
{
	sf::Vector2i direction;
	unsigned int LeftIsPress = 0;
	unsigned int RightIsPress = 0;
	sf::Vector2i mousePos;
	entt::entity entity;
};