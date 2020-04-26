#pragma once
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include <vector>
struct DrawDebugComponent
{
	float dt = 0.0f;
	std::vector<b2Vec2> vertices;
	sf::Color color = sf::Color(sf::Uint8(255), sf::Uint8(100), sf::Uint8(100), sf::Uint8(128));
};
struct DebugEntities : public sf::Drawable
{
	sf::VertexArray vArray;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(vArray, states);
	}
};