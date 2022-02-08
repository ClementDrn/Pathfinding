#include "PathFinding.h"

using namespace std;
using namespace sf;


void addToCellList(PathCell* cell, vector<vector<PathCell*>>& list)
{
	bool found(false);
	int i(0);
	while (i < list.size() && !found)
	{
		if (list[i].size() != 0) {
			if (list[i][0]->y == cell->y) {
				found = true;
				//break;
			}
		}
		if (!found)
			i++;
	}
	if (found) {
		found = false;
		int j = 0;
		//for (int j = 0; j < list[i].size(); j++)
		while (j < list[i].size() && !found)
		{
			if (list[i][j]->x == cell->x) {
				found = true;
				list[i][j] = cell;
				//break;
			}
			j++;
		}
		if (!found) {
			list[i].push_back(cell);
		}
	}
	else {
		list.push_back({ cell });
	}

}

void setGHF(PathCell* cell, const PathCell* previousCell, const Position2D& target, bool prevCellIsClose)
{
	if (prevCellIsClose)
		cell->G = previousCell->G + 1;
	else {
		int gX(target.x - cell->x), gY(target.y - cell->y);
		if (gX < 0)
			gX *= -1;
		if (gY < 0)
			gY *= -1;
		cell->G = gX + gY;
	}

	int hX(target.x - cell->x), hY(target.y - cell->y);
	if (hX < 0)
		hX *= -1;
	if (hY < 0)
		hY *= -1;
	cell->H = hX + hY;

	cell->F = cell->G + cell->H;
}

PathCell* createPathCell(int x, int y, const PathCell* start, const Position2D& target)
{
	PathCell* cell = new PathCell;
	cell->x = x;
	cell->y = y;
	
	setGHF(cell, start, target);

	return cell;
}

Bool_Position2D cellInList(Position2D& cell, vector<vector<PathCell*>>& list)
{
	Bool_Position2D output;
	output.boolean = false;
	int i(0);
	bool stop(false);
	while (i < list.size() && !stop)
	{
		if (list[i].size() != 0 && list[i][0]->y == cell.y)
		{
			int j(0);
			while (!output.boolean && j < list[i].size())
			{
				if (list[i][j]->x == cell.x)
					output.boolean = true;
				if (!output.boolean)
					j++;
			}
			output.position.x = j; output.position.y = i;
			stop = true;
		}
		i++;
	}
	return output;
}

void setCellNeighbours(PathCell* current, vector<vector<PathCell*>>& openCells, vector<vector<PathCell*>>& closeCells, const vector<string>& grid, const Position2D& target)
{

	vector<Position2D> vec = { {-1, 0}, {0, -1}, {0, 1}, {1, 0} };
	for (auto& const coos : vec) {
		Position2D neighbourCoos; neighbourCoos.x = current->x + coos.x; neighbourCoos.y = current->y + coos.y;

		// Detect drawing mode issues
		if (neighbourCoos.y < 0 || neighbourCoos.x < 0 || neighbourCoos.y >= grid.size() || neighbourCoos.x >= grid[0].size())
			continue;
			
		if (grid[neighbourCoos.y][neighbourCoos.x] == '#' || cellInList(neighbourCoos, closeCells).boolean)
			continue;

		PathCell* neighbour = new PathCell;
		neighbour->x = neighbourCoos.x; neighbour->y = neighbourCoos.y;
		setGHF(neighbour, current, target);

		Bool_Position2D bool_pos = cellInList(neighbourCoos, openCells);
		// If neighbour not in openCells OR new path to neighbour (F) is shorter
		if (!bool_pos.boolean || neighbour->F < openCells[bool_pos.position.y][bool_pos.position.x]->F)
		{
			neighbour->parent = current;
			// If it is not in openCells
			if (!bool_pos.boolean) {
				if (bool_pos.position.y >= openCells.size())
					addToCellList(neighbour, openCells);
				else
					openCells[bool_pos.position.y].push_back(neighbour);
			}
			else {
				openCells[bool_pos.position.y][bool_pos.position.x] = neighbour;	// Replaces the old cell by the one with the shorter path
			}
		}
	}
}

void showStats(std::vector<std::vector<PathCell*>>& list, sf::Vector2u& winSize, RenderWindow& window, const int zoom, Color color, Font &font)
{
	for (auto& const row : list) {
		for (auto& const cell : row)
		{
			unsigned x(cell->x * zoom), y(cell->y * zoom);
			RectangleShape square(Vector2f(zoom, zoom));
			square.setFillColor(color);
			square.setPosition(Vector2f(x, y));

			Text textG(to_string(cell->G), font, 10);
			Text textH(to_string(cell->H), font, 10);
			Text textF(to_string(cell->F), font, 16);

			textG.setPosition(Vector2f(x, y));
			textH.setPosition(Vector2f(x, y + zoom - 10));
			textF.setPosition(Vector2f(x + zoom/2 - 10, y + zoom/2 - 8));

			textG.setFillColor(Color::White);
			textH.setFillColor(Color::White);
			textF.setFillColor(Color::White);

			window.draw(square);
			window.draw(textG);
			window.draw(textH);
			window.draw(textF);
		}
	}
}


vector<Position2D> findPath(const vector<string> &grid, const Position2D &startPosition, const Position2D &target, Vector2u& winSize, RenderWindow& window, const int zoom, Font &font, bool debugMode)
//vector<Position2D> findPath(const vector<string> &grid, const Position2D &startPosition, const Position2D &target)
{
	vector<Position2D> path;

	vector<vector<PathCell*>> openCells, closeCells;

	PathCell* start = new PathCell;
	PathCell* current = new PathCell;
	start->x = startPosition.x, start->y = startPosition.y;

	setGHF(start, start, target, false);
	addToCellList(start, openCells);

	// Loop
	bool pathFound(false);
	while (!pathFound)
	{
		
		// current = cell in openCells with the lowest F cost (lowest H cost if equal)
		bool currentUninitialized(true), stop(false);
		Position2D currentIndex;
		int i(0);
		while (i < openCells.size() && !stop)
		{
			int j(0);
			while (j < openCells[i].size())
			{
				if (currentUninitialized) {
					current = openCells[i][j];
					currentIndex.x = j; currentIndex.y = i;
					currentUninitialized = false;
				}
				else if (openCells[i][j]->F < current->F || (openCells[i][j]->F == current->F && openCells[i][j]->H < current->H)) {
					current = openCells[i][j];
					currentIndex.x = j; currentIndex.y = i;
				}
				j++;
			}
			i++;
		}
		if (currentUninitialized) {
			cout << "Error: No cell is openCells could be set as the current cell.\n";
			stop = true;
			//break;
		}
		else
		{
			// Remove the current cell from openCells
			openCells[currentIndex.y].erase(openCells[currentIndex.y].begin() + currentIndex.x);
			// Add the current cell to closeCells
			addToCellList(current, closeCells);


			if (current->x == target.x && current->y == target.y)
				pathFound = true;		// The target is reached
			else
			{
				setCellNeighbours(current, openCells, closeCells, grid, target);
			}

			// Debug Mode
			if (debugMode)
			{
				showStats(openCells, winSize, window, zoom, Color(0, 182, 28), font);
				showStats(closeCells, winSize, window, zoom, Color(201, 17, 3), font);
				window.display();
				cin.get();

			}
		}
	}

	PathCell* step = current;
	while (step->x != start->x || step->y != start->y)
	{
		Position2D stepCoos; stepCoos.x = step->x; stepCoos.y = step->y;
		path.push_back(stepCoos);
		step = step->parent;
	}

	for (auto& const list : {openCells, closeCells}) {
		for (auto& const row : list) {
			for (auto& const ptr : row)
				delete ptr;
		}
	}

	return path;
}
