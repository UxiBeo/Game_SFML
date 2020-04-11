#include "Graphics.h"

void Graphics::MoveViewport(const b2Vec2& newWorldPos, const sf::Vector2i& mousePos)
{
	auto view = window.getView();
	auto viewPos = view.getCenter();
	auto targetPos = (cameraRelative * WorldToScreenPos(newWorldPos)) + ((1.0f - cameraRelative) * window.mapPixelToCoords(mousePos));
	viewPos += cameraMovespeed * (targetPos - viewPos);
	
	view.setCenter(viewPos);
	window.setView(view);
}
