#pragma once
#include <SFML/Graphics.hpp>
#include "Mouse.h"
#include "Keyboard.h"
class MainWindow
{
public:
	bool Update(Mouse& mouse, Keyboard& kbd, sf::RenderWindow& window)
	{
		if (window.isOpen())
		{
			ProcessEvent(mouse, kbd, window);
			return true;
		}
		return false;
	}	
private:
	void ProcessEvent(Mouse& mouse, Keyboard& kbd, sf::RenderWindow& window);
};