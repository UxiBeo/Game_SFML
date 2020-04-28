#pragma once
#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include <assert.h>
class Graphics
{
public:
	Graphics()
		:
		window(sf::VideoMode(1280, 720), "SFML Game")
	{
		//enable v-sync
		window.setVerticalSyncEnabled(true);
		//disable key auto repeat
		window.setKeyRepeatEnabled(false);

	}
	void DrawVertexArray(sf::VertexArray& vertexArray, const sf::Texture& texture)
	{
		window.draw(vertexArray, &texture);
	}
	void Draw(const sf::Drawable& drawable)
	{
		window.draw(drawable);
	}
	void DrawSprite(const sf::Sprite& sprite)
	{
		window.draw(sprite);
	}
	void BeginFrame()
	{
		window.clear(sf::Color::Black);
	}
	void EndFrame()
	{
		window.display();
	}
	//std::pair<b2Vec2, b2Vec2> GetViewport() const
	//{
	//	const auto center = window.getView().getCenter();
	//	halfSize = 0.5f * window.getView().getSize();
	//	const auto top = ScreenToWorldPos(center - halfSize);
	//	const auto bottom = ScreenToWorldPos(center + halfSize);
	//	/*auto draw01 = WorldToScreenPos(top) + sf::Vector2f(10.0f,10.0f);
	//	auto draw02 = WorldToScreenPos(bottom) - sf::Vector2f(10.0f, 10.0f);
	//	vertices[0] = draw01;
	//	vertices[1] = sf::Vector2f(draw01.x,draw02.y);
	//	vertices[2] = draw02;
	//	vertices[3] = sf::Vector2f(draw02.x, draw01.y);
	//	window.draw(vertices);*/
	//	return { top, bottom };
	//}
	/*std::pair<sf::Vector2f, sf::Vector2f> GetViewportScreen() const
	{
		const auto center = window.getView().getCenter();
		halfSize = 0.5f * window.getView().getSize();
		return { center - halfSize , center + halfSize };
	}*/
	void MoveViewport(const b2Vec2& newWorldPos, const sf::Vector2i& mousePos);
	void MoveViewport(const sf::Vector2f screenPos)
	{
		auto view = window.getView();

		view.setCenter(view.getCenter() + screenPos);
		window.setView(view);
	}
	sf::Vector2f GetViewportLocation() const
	{
		return window.getView().getCenter();
	}
	b2Vec2 ScreenToWorldPos(const sf::Vector2f& screenPos) const
	{
		return b2Vec2((screenPos.x) / scalePixel, (-screenPos.y) / scalePixel);
	}
	b2Vec2 MouseToWorldPos(const sf::Vector2i& mousePos) const
	{
		const auto newPosition = window.mapPixelToCoords(mousePos);

		return b2Vec2((newPosition.x) / scalePixel, (-newPosition.y) / scalePixel);
	}
	static sf::Vector2f WorldToScreenPos(const b2Vec2& worldPos)
	{
		return { worldPos.x * scalePixel, - worldPos.y * scalePixel};
	}
	b2Vec2 GetWorldSize(const sf::Vector2f& screenSize) const
	{
		return b2Vec2(screenSize.x / scalePixel, screenSize.y / scalePixel);
	}
	sf::Vector2f GetScreenSize(const b2Vec2& worldSize) const
	{
		return { worldSize.x * scalePixel, worldSize.y * scalePixel };
	}
	sf::View GetView() const
	{
		return window.getView();
	}
	static constexpr float scalePixel = 20.0f;
	sf::RenderWindow& getRenderWindow()
	{
		return window;
	}
private:
	sf::RenderWindow window;
	sf::Vector2f halfSize;
	sf::VertexArray vertices;
	float cameraRelative = 0.52f;
	float cameraMovespeed = 0.1f;
};