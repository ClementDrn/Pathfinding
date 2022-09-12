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

		static std::vector<sel::Vec2u> findPathInGrid(const bool* grid, const sel::Vec2u& gridSize, const sel::Vec2u& start, const sel::Vec2u& goal);

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
