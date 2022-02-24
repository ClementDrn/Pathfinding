#include <Pathfinding/AStar.hpp>


namespace Pathfinding {

	AStar::Cell::Cell(const sel::Vec2u& coords)
		: m_coords(coords)
	{
	}

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

}
