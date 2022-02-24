#pragma once

#include <SEL/Vec2.hpp>
#include <SEL/Reference.hpp>
#include <SEL/Container.hpp>

#include <array>
#include <vector>
#include <queue>
#include <forward_list>


namespace Pathfinding {

	class AStar
	{
	public:

		// --- AStar Cell ---------------------------------

		class Cell
		{
		public:

			Cell() = default;
			Cell(const sel::Vec2u& coords);

			void update(const sel::Ref<Cell>& previous, const sel::Vec2u& goal);
			
			sel::Vec2u getCoords() const { return m_coords; }
			sel::Ref<Cell> getParent() const { return m_parent; };

			uint32_t getCostFromStart() const { return m_g; }
			uint32_t getHeuristic() const { return m_h; }
			uint32_t calcTotalCost() const { return m_g + m_h; }

			void setCostFromStart(int g) { m_g = g; }
			void setHeuristic(int h) { m_h = h; }

			friend bool operator<(const Cell& lhs, const Cell& rhs);
			friend bool operator==(const Cell& lhs, const Cell& rhs);

		private:

			sel::Vec2u m_coords;
			uint32_t m_g = 0;
			uint32_t m_h = 0;
			sel::Ref<Cell> m_parent = nullptr;
		};


		struct IsCellRefEqual
		{
		public:

			IsCellRefEqual(sel::Vec2u coords)
				: m_coords(coords) {}

			bool operator()(const sel::Ref<Cell>& other) const
			{
				return m_coords == other->getCoords();
			}

		private:

			sel::Vec2u m_coords;
		};

		struct IsCellRefLesser
		{
			bool operator() (const sel::Ref<Cell>& lhs, const sel::Ref<Cell>& rhs) const
			{
				return *lhs < *rhs;
			}
		};


		// --- Pathfinding in grid ------------------------

		template <size_t sizeX, size_t sizeY>
		static std::vector<sel::Vec2u> findPathInGrid(std::array<std::array<bool, sizeY>, sizeX> grid, const sel::Vec2u& start, const sel::Vec2u& goal)
		{
			std::vector<sel::Vec2u> path;

			// List of cells that need their costs to be calculated.
			std::priority_queue<sel::Ref<Cell>, std::vector<sel::Ref<Cell>>, IsCellRefLesser> openSet;
			std::vector<sel::Ref<Cell>>& openContainer = sel::container(openSet);

			// Costs of cells in closedSet do not need to be recalculated, they are the best possible.
			std::forward_list<sel::Ref<Cell>> closedSet;

			sel::Ref<Cell> startCell = sel::createRef<Cell>(start);

			startCell->update(nullptr, goal);
			openSet.push(startCell);

			while (!openSet.empty())
			{
				sel::Ref<Cell> currentCell = openSet.top();
				// If the goal has been reached, we reconstruct the path and return it.
				sel::Vec2u coords = currentCell->getCoords();
				if (coords == goal)
				{
					int length = currentCell->getCostFromStart();
					path.reserve(length);

					while (currentCell->getParent() != nullptr)
					{
						path.push_back(currentCell->getCoords());
						currentCell = currentCell->getParent();
					}

					std::reverse(std::begin(path), std::end(path));

					return path;
				}

				// If the goal has not been reached yet, we continue the algorithm.
				openSet.pop();
				closedSet.push_front(currentCell);
				
				const int neighbourShifts[4][2] = { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
				for (const auto& shift : neighbourShifts)
				{
					sel::Vec2u neighbourCoords(coords.x + shift[0], coords.y + shift[1]);

					// Prevents out of range errors
					if (neighbourCoords.x >= sizeX || neighbourCoords.x < 0 || neighbourCoords.y >= sizeY || neighbourCoords.y < 0)
						continue;

					sel::Ref<Cell> neighbour = sel::createRef<Cell>(sel::Vec2u{ coords.x + shift[0], coords.y + shift[1] });

					// If neighbour not traversable or is is closedSet, we skip to next neighbour.
					if (!grid[neighbourCoords.x][neighbourCoords.y]
						|| std::find_if(std::begin(closedSet), std::end(closedSet), IsCellRefEqual(neighbourCoords)) != std::end(closedSet))
						continue;
						
					// Try to find neighbour in openSet.
					auto neighbourIt = std::find_if(std::begin(openContainer), std::end(openContainer), IsCellRefEqual(neighbourCoords));
					
					// If neighbour is not in openSet we create it, update it and add it to openSet.
					if (neighbourIt == std::end(openContainer))
					{
						sel::Ref<Cell> neighbour = sel::createRef<Cell>(neighbourCoords);
						neighbour->update(currentCell, goal);
						openSet.push(neighbour);
					}
					// If neighbour is already in openSet but the path to it through current is shorter, then we update it.
					else
					{
						sel::Ref<Cell> neighbour = *neighbourIt;
						if (currentCell->calcTotalCost() + 1 < neighbour->calcTotalCost())
						{
							openContainer.erase(neighbourIt);
							neighbour->update(currentCell, goal);
							openSet.push(neighbour);
						}
					}
				}
			}

			// Error: No path could be found.
			return path;
		}

	};


	// --- AStar::Cell operator overloads -----------------

	// Test coordinates
	inline bool operator==(const AStar::Cell& lhs, const AStar::Cell& rhs) 
	{
		return lhs.m_coords == rhs.m_coords;
	}

	inline bool operator!=(const AStar::Cell& lhs, const AStar::Cell& rhs) { return !operator==(lhs, rhs); }
	
	// Useful for the prority_queue: the cell at the top has the lowest F (and lowest H if there are multiple same Fs).
	inline bool operator< (const AStar::Cell& lhs, const AStar::Cell& rhs)
	{
		int lhsF = lhs.calcTotalCost();
		int rhsF = rhs.calcTotalCost();

		if (lhsF != rhsF)
			return lhsF > rhsF;
		else
			return lhs.m_h > rhs.m_h;
	}

	inline bool operator> (const AStar::Cell& lhs, const AStar::Cell& rhs) { return  operator< (rhs, lhs); }
	inline bool operator<=(const AStar::Cell& lhs, const AStar::Cell& rhs) { return !operator> (lhs, rhs); }
	inline bool operator>=(const AStar::Cell& lhs, const AStar::Cell& rhs) { return !operator< (lhs, rhs); }

}
