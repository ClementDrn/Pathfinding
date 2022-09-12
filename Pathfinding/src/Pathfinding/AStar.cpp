#include <Pathfinding/AStar.hpp>


namespace Pathfinding {

	AStar::Cell::Cell(const sel::Vec2u& coords)
		: m_coords(coords) {}

	void AStar::Cell::update(const sel::Ref<Cell>& previous, const sel::Vec2u& goal)
	{
		m_parent = previous;
		
		// --- Updates G ---
		
		// If the cell has no parent, then it is the start and has a cost from start (g) of 0.
		if (previous)
			m_g = previous->m_g + 1;
		else
			m_g = 0;

		// --- Updated H ---

		int hX = goal.x - m_coords.x;
		int hY = goal.y - m_coords.y;

		// We don't want negative values.
		if (hX < 0)
			hX *= -1;
		if (hY < 0)
			hY *= -1;

		m_h = (uint32_t)(hX + hY);
	}

	std::vector<sel::Vec2u> AStar::findPathInGrid(const bool* grid, const sel::Vec2u& gridSize, const sel::Vec2u& start, const sel::Vec2u& goal)
	{
		std::vector<sel::Vec2u> path;

		// List of cells that need their costs to be calculated.
		std::priority_queue<sel::Ref<Cell>, std::vector<sel::Ref<Cell>>, IsCellRefLesser> openSet;
		std::vector<sel::Ref<Cell>>& openContainer = sel::container(openSet);

		// Costs of cells in closedSet do not need to be recalculated, they are the best possible.
		std::forward_list<sel::Ref<Cell>> closedSet;

		sel::Ref<Cell> startCell = createRef<Cell>(start);

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
				if (neighbourCoords.x >= gridSize.x || neighbourCoords.x < 0 || neighbourCoords.y >= gridSize.y || neighbourCoords.y < 0)
					continue;

				sel::Ref<Cell> neighbour = sel::createRef<Cell>(sel::Vec2u{ coords.x + shift[0], coords.y + shift[1] });

				// If neighbour not traversable or is is closedSet, we skip to next neighbour.
				if (!grid[neighbourCoords.x + neighbourCoords.y * gridSize.x]
					|| std::find_if(std::begin(closedSet), std::end(closedSet), IsCellRefEqual(neighbourCoords)) != std::end(closedSet))
					continue;

				// Try to find neighbour in openSet.
				auto neighbourIt = std::find_if(std::begin(openContainer), std::end(openContainer), IsCellRefEqual(neighbourCoords));

				// If neighbour is not in openSet we create it, update it and add it to openSet.
				if (neighbourIt == std::end(openContainer))
				{
					sel::Ref<Cell> neighbour = createRef<Cell>(neighbourCoords);
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

}
