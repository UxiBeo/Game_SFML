#pragma once
#include "World.h"

class Game
{
public:
	void Go()
	{
		Locator::Graphic::ref().BeginFrame();
		UpdateModel();
		ComposeFrame();
		Locator::Graphic::ref().EndFrame();
	}
private:
	void UpdateModel()
	{
		wld.Update();
	}
	void ComposeFrame()
	{
		wld.Draw();
	}
private:
	/**************** User Variable *******************/

	World wld;
	/**************** User Variable *******************/
};