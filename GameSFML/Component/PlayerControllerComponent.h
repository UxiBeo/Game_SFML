#pragma once
#include "SFML/System/Vector2.hpp"
#include "box2d/box2d.h"
struct PlayerControllerComponent
{
	sf::Vector2i direction;
	unsigned int LeftIsPress = 0;
	sf::Vector2i mouseScreenPos;
	b2Vec2 mouseWorldPos;
};