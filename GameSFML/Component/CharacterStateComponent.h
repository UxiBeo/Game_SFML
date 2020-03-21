#pragma once
#include "entt/entt.hpp"
#include "../HashStringDataBase.h"
#include "../Component/PlayerControllerComponent.h"
struct CharacterStateComponent
{
	enum State
	{
		IdleAndWalking,
		Shooting,
		Dead,
	};

	State curState = State::IdleAndWalking;

	State prevState = curState;
	entt::hashed_string prevAnim{};
	entt::hashed_string curAnim{};
	sf::Vector2i prevDir = { 0,0 };
	sf::Vector2i curDir = { 0,0};
	bool canMove = true;
};