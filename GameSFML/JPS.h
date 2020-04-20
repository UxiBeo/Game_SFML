#pragma once
#include "Grid.h"
#include <optional>
#include <unordered_map>

namespace JPS
{
	struct Node 
	{
		bool operator==(const Node& b) const
		{
			return x == b.x && y == b.y;
		}
		static bool CompareFCost(const Node& left, const Node& right)
		{
			return left.f > right.f;
		}
		int x;
		int y;
		int parent = -1;
		int f = 0;
		int g = 0;
		int h = 0;
		bool opened = false;
		bool closed = false;
	};
	class Queue
	{
	public:
		int PopHeap()
		{
			std::pop_heap(openList.begin(), openList.end(), [this](int i1, int i2) {
				return Node::CompareFCost(GetNode(i1), GetNode(i2));
				});
			int index = openList.back();
			openList.pop_back();
			return index;
		}
		std::optional<std::vector<sf::Vector2i>> backtrace(int endE)
		{
			std::vector<sf::Vector2i> path;
			const auto& node = GetNode(endE);
			path.emplace_back(node.x, node.y);
			int parentE = node.parent;
			while (parentE >= 0) {
				const auto& nodeP = GetNode(parentE);
				parentE = nodeP.parent;
				path.emplace_back(nodeP.x, nodeP.y);
			}
			std::reverse(path.begin(), path.end());
			Clear();
			return path;
		}
		Node& GetNode(int index)
		{
			return pool[tracker[index]];
		}
		const Node& ReadNode(int index) const
		{
			return pool[tracker.at(index)];
		}
		bool Has(int index) const
		{
			return tracker.count(index) > 0;
		}
		void PushHeap(int index, const Node& data)
		{
			emplace(index, data);
			openList.push_back(index);
			std::push_heap(openList.begin(), openList.end(), [this](int i1, int i2) {
				return Node::CompareFCost(GetNode(i1), GetNode(i2));
				});
		}
		void UpdateHeap()
		{
			std::make_heap(openList.begin(), openList.end(), [this](int i1, int i2) {
				return Node::CompareFCost(GetNode(i1), GetNode(i2));
				});
		}
		void emplace(int index, const Node& data)
		{
			pool.push_back(data);
			tracker.emplace(index, (int)(pool.size() - 1));
		}
		bool isOpenlistEmpty() const
		{
			return openList.empty();
		}
		size_t PoolSize() const
		{
			return pool.size();
		}
		size_t openSize() const
		{
			return openList.capacity();
		}
	private:
		void Clear()
		{
			pool.clear();
			tracker.clear();
			openList.clear();
		}
	private:
		std::vector<Node> pool;
		std::unordered_map<int, int> tracker;
		std::vector<int> openList;
	};
	class Searcher
	{
	public:
		std::optional<std::vector<sf::Vector2i>> findPath(const sf::Vector2i& start, const sf::Vector2i& end, const Grid* searchGrid) 
		{
			grid = searchGrid;
			endPos = end;
			startIndex = grid->GetIndexAt(start.x, start.y);
			endIndex = grid->GetIndexAt(endPos.x, endPos.y);

			// set the `g` and `f` value of the start node to be 0
			Node startNode;
			startNode.x = start.x;
			startNode.y = start.y;
			// push the start node into the open list
			startNode.opened = true;
			queue.PushHeap(startIndex, startNode);
			// while the open list is not empty
			while (!queue.isOpenlistEmpty()) {
				// pop the position of node which has the minimum `f` value.
				int nodeIndex = queue.PopHeap();
				Node& node = queue.GetNode(nodeIndex);

				node.closed = true;

				if (nodeIndex == endIndex) {
					return queue.backtrace(endIndex);
				}

				_identifySuccessors(node, nodeIndex);
			}
			return std::nullopt;
		};
	private:
		std::optional<sf::Vector2i> _jump(int x, int y, int px, int py) const
		{
			const int dx = x - px;
			const int dy = y - py;
			const sf::Vector2i pos = { x,y };

			if (!grid->isWalkableAt(x, y))
			{
				return std::nullopt;
			}

			if (grid->GetIndexAt(x, y) == endIndex)
			{
				return std::optional<sf::Vector2i>{pos};
			}

			// check for forced neighbors
			// along the diagonal
			if (dx != 0 && dy != 0)
			{
				// if ((grid->isWalkableAt(x - dx, y + dy) && !grid->isWalkableAt(x - dx, y)) ||
				// (grid->isWalkableAt(x + dx, y - dy) && !grid->isWalkableAt(x, y - dy))) {
				// return [x, y];
				// }
				// when moving diagonally, must check for vertical/horizontal jump points
				if (_jump(x + dx, y, x, y) || _jump(x, y + dy, x, y))
				{
					return std::optional<sf::Vector2i>{pos};
				}
			}
			// horizontally/vertically
			else
			{
				if (dx != 0)
				{
					if ((grid->isWalkableAt(x, y - 1) && !grid->isWalkableAt(x - dx, y - 1)) ||
						(grid->isWalkableAt(x, y + 1) && !grid->isWalkableAt(x - dx, y + 1)))
					{
						return std::optional<sf::Vector2i>{pos};
					}
				}
				else if (dy != 0)
				{
					if ((grid->isWalkableAt(x - 1, y) && !grid->isWalkableAt(x - 1, y - dy)) ||
						(grid->isWalkableAt(x + 1, y) && !grid->isWalkableAt(x + 1, y - dy)))
					{
						return std::optional<sf::Vector2i>{pos};
					}
					// When moving vertically, must check for horizontal jump points
					// if (this._jump(x + 1, y, x, y) || this._jump(x - 1, y, x, y)) {
					// return [x, y];
					// }
				}
			}

			// moving diagonally, must make sure one of the vertical/horizontal
			// neighbors is open to allow the path
			if (grid->isWalkableAt(x + dx, y) && grid->isWalkableAt(x, y + dy))
			{
				return _jump(x + dx, y + dy, x, y);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::vector<sf::Vector2i> _findNeighbors(const Node& node) const
		{
			const int x = node.x, y = node.y;
			std::vector<sf::Vector2i> neighbors;

			// directed pruning: can ignore most neighbors, unless forced.
			if (node.parent >= 0)
			{
				const Node& np = queue.ReadNode(node.parent);
				// get the normalized direction of travel
				const int dx = (x - np.x) / std::max(abs(x - np.x), 1);
				const int dy = (y - np.y) / std::max(abs(y - np.y), 1);

				// search diagonally
				if (dx != 0 && dy != 0)
				{
					if (grid->isWalkableAt(x, y + dy)) {
						neighbors.emplace_back(x, y + dy);
					}
					if (grid->isWalkableAt(x + dx, y)) {
						neighbors.emplace_back(x + dx, y);
					}
					if (grid->isWalkableAt(x, y + dy) && grid->isWalkableAt(x + dx, y)) {
						neighbors.emplace_back(x + dx, y + dy);
					}
				}
				// search horizontally/vertically
				else
				{
					bool isNextWalkable;
					if (dx != 0)
					{
						isNextWalkable = grid->isWalkableAt(x + dx, y);
						bool isTopWalkable = grid->isWalkableAt(x, y + 1);
						bool isBottomWalkable = grid->isWalkableAt(x, y - 1);

						if (isNextWalkable)
						{
							neighbors.emplace_back(x + dx, y);
							if (isTopWalkable) neighbors.emplace_back(x + dx, y + 1);
							if (isBottomWalkable) neighbors.emplace_back(x + dx, y - 1);
						}
						if (isTopWalkable) neighbors.emplace_back(x, y + 1);
						if (isBottomWalkable) neighbors.emplace_back(x, y - 1);
					}
					else if (dy != 0)
					{
						isNextWalkable = grid->isWalkableAt(x, y + dy);
						bool isRightWalkable = grid->isWalkableAt(x + 1, y);
						bool isLeftWalkable = grid->isWalkableAt(x - 1, y);

						if (isNextWalkable)
						{
							neighbors.emplace_back(x, y + dy);
							if (isRightWalkable) neighbors.emplace_back(x + 1, y + dy);
							if (isLeftWalkable) neighbors.emplace_back(x - 1, y + dy);
						}
						if (isRightWalkable) neighbors.emplace_back(x + 1, y);
						if (isLeftWalkable) neighbors.emplace_back(x - 1, y);
					}
				}
			}
			// return all neighbors
			else
			{
				auto neighborNodes = grid->getNeighbors(node.x, node.y);
				for (auto& n : neighborNodes)
				{
					neighbors.push_back(n);
				}
			}

			return neighbors;
		};

		void _identifySuccessors(const Node& node, int parentEnttity)
		{
			const int x = node.x;
			const int y = node.y;

			for (sf::Vector2i& neighbor : _findNeighbors(node))
			{
				const auto jumpPoint = _jump(neighbor.x, neighbor.y, x, y);
				if (jumpPoint)
				{
					const int jx = jumpPoint->x;
					const int jy = jumpPoint->y;
					auto entity = grid->GetIndexAt(jx, jy);
					if (queue.Has(entity))
					{
						auto& jumpNode = queue.GetNode(entity);
						if (jumpNode.closed) continue;

						// include distance, as parent may not be immediately adjacent:
						const int d = abs(jx - x) + abs(jy - y);
						const int ng = node.g + d; // next `g` value

						if (ng < jumpNode.g)
						{
							jumpNode.g = ng;
							jumpNode.h = abs(jx - endPos.x) + abs(jy - endPos.y);
							jumpNode.f = jumpNode.g + jumpNode.h;
							jumpNode.parent = parentEnttity;
							queue.UpdateHeap();
						}
					}
					else
					{
						// include distance, as parent may not be immediately adjacent:
						const int d = abs(jx - x) + abs(jy - y);
						const int ng = node.g + d; // next `g` value
						Node jumpNode;
						jumpNode.x = jx;
						jumpNode.y = jy;
						jumpNode.g = ng;
						jumpNode.h = abs(jx - endPos.x) + abs(jy - endPos.y);
						jumpNode.f = jumpNode.g + jumpNode.h;
						jumpNode.parent = parentEnttity;
						jumpNode.opened = true;
						queue.PushHeap(entity, jumpNode);
					}
				}
			}
		}
	private:
		const Grid* grid = nullptr;
		Queue queue;
		sf::Vector2i endPos;
		int startIndex;
		int endIndex;
	};
}
