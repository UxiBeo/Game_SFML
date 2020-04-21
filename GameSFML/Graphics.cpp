#include "Graphics.h"
#include <cmath>
void Graphics::MoveViewport(const b2Vec2& newWorldPos, const sf::Vector2i& mousePos)
{
	cameraRelative = 0.6;
	cameraMovespeed = 0.1f;
	auto view = window.getView();
	auto viewPos = view.getCenter();
	auto targetPos = (cameraRelative * WorldToScreenPos(newWorldPos)) + ((1.0f - cameraRelative) * window.mapPixelToCoords(mousePos));
	viewPos += cameraMovespeed * (targetPos - viewPos);
	
	view.setCenter(sf::Vector2f(std::roundf(viewPos.x), std::roundf(viewPos.y)));
	window.setView(view);
}
