#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

struct PathCell {
	int x;
	int y;
	int G;
	int H;
	int F;
	PathCell* parent;
};

struct Position2D {
	int x;
	int y;
};

struct Bool_Position2D {
	bool boolean;
	Position2D position;
};

void addToCellList(PathCell* cell, std::vector<std::vector<PathCell*>>& list);

void setGHF(PathCell* cell, const PathCell* previousCell, const Position2D& target, bool prevCellIsClose = true);

PathCell* createPathCell(int x, int y, const PathCell* start, const Position2D &target);

Bool_Position2D cellInList(Position2D& cell, std::vector<std::vector<PathCell*>>& list);

void setCellNeighbours(PathCell* current, std::vector<std::vector<PathCell*>>& openCells, std::vector<std::vector<PathCell*>>& closeCells, const std::vector<std::string>& grid, const Position2D& target);

//std::vector<Position2D> findPath(const std::vector<std::string>& grid, const Position2D& startPosition, const Position2D &target);
std::vector<Position2D> findPath(const std::vector<std::string>& grid, const Position2D& startPosition, const Position2D &target, sf::Vector2u& winSize, sf::RenderWindow& window, const int zoom, sf::Font &font, bool debugMode = false);

void showStats(std::vector<std::vector<PathCell*>>& list, sf::Vector2u& winSize, sf::RenderWindow& window, const int zoom, sf::Color color, sf::Font &font);