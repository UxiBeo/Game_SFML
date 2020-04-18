#pragma once
#include "entt/core/hashed_string.hpp"
#include "SFML/Graphics.hpp"
class Grid
{
public:
	void Load(entt::hashed_string filename)
	{

	}
	bool isWalkableAt(int x, int y) const
	{
		if (isInside(x, y))
		{
			return nodes[GetIndexAt(x, y)];
		}
		return false;
	};
	uint32_t GetIndexAt(int x, int y) const
	{
		return y * width + x;
	}
	bool isInside(int x, int y) const
	{
		return (x >= 0 && x < width) && (y >= 0 && y < height);
	};
	void setWalkableAt(int x, int y, bool walkable)
	{
		nodes[GetIndexAt(x, y)] = walkable;
	};
	std::vector<sf::Vector2i> getNeighbors(uint32_t x, uint32_t y) const
	{
		std::vector<sf::Vector2i> neighbors;
		bool s0 = false;
		bool s1 = false;
		bool s2 = false;
		bool s3 = false;
		// ↑
		if (isWalkableAt(x, y - 1))
		{
			neighbors.emplace_back(x, y - 1);
			s0 = true;
		}
		// →
		if (isWalkableAt(x + 1, y)) {
			neighbors.emplace_back(x + 1, y);
			s1 = true;
		}
		// ↓
		if (isWalkableAt(x, y + 1)) {
			neighbors.emplace_back(x, y + 1);
			s2 = true;
		}
		// ←
		if (isWalkableAt(x - 1, y)) {
			neighbors.emplace_back(x - 1, y);
			s3 = true;
		}

		// ↖
		if (s3 && s0 && isWalkableAt(x - 1, y - 1)) {
			neighbors.emplace_back(x - 1, y - 1);
		}
		// ↗
		if (s0 && s1 && isWalkableAt(x + 1, y - 1)) {
			neighbors.emplace_back(x + 1, y - 1);
		}
		// ↘
		if (s1 && s2 && isWalkableAt(x + 1, y + 1)) {
			neighbors.emplace_back(x + 1, y + 1);
		}
		// ↙
		if (s2 && s3 && isWalkableAt(x - 1, y + 1)) {
			neighbors.emplace_back(x - 1, y + 1);
		}

		return neighbors;
	};
private:
	uint32_t width;
	uint32_t height;
	std::vector<bool> nodes;
};