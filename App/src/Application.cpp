#include "Application.hpp"

#include <Pathfinding/AStar.hpp>

#include <iostream>


namespace App {

    Application* Application::s_instance = nullptr;


	Application::Application()
        : sf::Application("Pathfinding A*", { .clearColor = sf::Color::White }), m_rendererTiles(), m_map()
	{
        // --- Setup map ------------------------

        // The map is empty (true everywhere).
		m_map.fill(true);

        // --- Setup grid for rendering ---------

        sf::Vector2u winSize = m_window->getSize();
        int i, j;

        // Vertical lines
        for (i = 0; i < MAP_SIZE_X; i++)
        {
            m_rendererGrid[i] = sf::RectangleShape({ 1.0f, (float)winSize.y });
            m_rendererGrid[i].setPosition({ i * winSize.x / (float)MAP_SIZE_X, 0.0f });
            m_rendererGrid[i].setFillColor(sf::Color::Black);
        }

        // Horizontal lines
        for (j = 0; j < MAP_SIZE_Y; j++)
        {
            m_rendererGrid[MAP_SIZE_X + j] = sf::RectangleShape({ (float)winSize.x, 1.0f });
            m_rendererGrid[MAP_SIZE_X + j].setPosition({ 0.0f, j * winSize.y / (float)MAP_SIZE_Y});
            m_rendererGrid[MAP_SIZE_X + j].setFillColor(sf::Color::Black);
        }
	}


	Application* Application::create()
	{
		if (!s_instance)
            s_instance = new Application();
		return s_instance;
	}

	Application* Application::get()
	{
		return s_instance;
	}

	void Application::destroy()
	{
		if (s_instance)
			delete s_instance;
	}


	void Application::onEvent(const sf::Event& e)
	{
        if (e.type == sf::Event::Closed)
            m_window->close();

        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::LControl)
                m_isObstacleMode = false;
            if (e.key.code == sf::Keyboard::Space)
                m_isSpacePressed = true;
        }
        if (e.type == sf::Event::KeyReleased)
        {
            if (e.key.code == sf::Keyboard::LControl)
                m_isObstacleMode = true;
            if (e.key.code == sf::Keyboard::Space)
                m_isSpacePressed = false;
        }

        if (e.type == sf::Event::MouseButtonPressed)
        {
            if (e.mouseButton.button == sf::Mouse::Right)
                m_isMouseRightPressed = true;
            if (e.mouseButton.button == sf::Mouse::Left)
                m_isMouseLeftPressed = true;
            m_mousePosition.x = e.mouseButton.x;
            m_mousePosition.y = e.mouseButton.y;
            m_needsUpdate = true;
        }

        if (e.type == sf::Event::MouseButtonReleased)
        {
            if (e.mouseButton.button == sf::Mouse::Right)
                m_isMouseRightPressed = false;
            if (e.mouseButton.button == sf::Mouse::Left)
                m_isMouseLeftPressed = false;
        }

        if (e.type == sf::Event::MouseMoved)
        {
            m_mousePosition.x = e.mouseMove.x;
            m_mousePosition.y = e.mouseMove.y;
            if (m_isMouseLeftPressed || m_isMouseRightPressed)
                m_needsUpdate = true;
        }
	}

    void Application::onUpdate(const sf::Time& ts)
    {
        if (m_needsUpdate)
        {
            int x = m_mousePosition.x / CELL_SIZE;
            int y = m_mousePosition.y / CELL_SIZE;
            if (x < MAP_SIZE_X && x >= 0 && y < MAP_SIZE_Y && y >= 0)
            {
                if (m_isObstacleMode) 
                {
                    if ((x != m_start.x || y != m_start.y) && (x != m_goal.x || y != m_goal.y))
                    {
                        if (m_isMouseLeftPressed)
                        {
                            m_rendererTiles[x][y] = sel::createRef<sf::RectangleShape>(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                            m_rendererTiles[x][y]->setFillColor(sf::Color(220, 30, 60));
                            m_rendererTiles[x][y]->setPosition({ (float)(x * CELL_SIZE), (float)(y * CELL_SIZE) });
                            m_map[y * MAP_SIZE_X + x] = false;
                        }
                        else if (m_isMouseRightPressed)
                        {
                            m_rendererTiles[x][y] = nullptr;
                            m_map[y * MAP_SIZE_X + x] = true;
                        }
                    }
                }
                else 
                {
                    if (m_isMouseLeftPressed) 
                    {
                        m_start.x = x;
                        m_start.y = y;
                    }
                    if (m_isMouseRightPressed) 
                    {
                        m_goal.x = x;
                        m_goal.y = y;
                    }

                    m_map[y * MAP_SIZE_X + x] = true;
                    m_rendererTiles[x][y] = nullptr;
                }

                m_path = Pathfinding::AStar::findPathInGrid(m_map.data(), { MAP_SIZE_X, MAP_SIZE_Y }, { m_start.x, m_start.y }, { m_goal.x, m_goal.y });

                m_needsUpdate = false;
            }
        }
    }

    void Application::onRender()
    {
        // Draw obstacles
        for (int x = 0; x < MAP_SIZE_X; x++)
        {
            for (int y = 0; y < MAP_SIZE_Y; y++)
            {
                // If the tile is an obstacle, then there is something to render.
                if (m_rendererTiles[x][y])
                {
                    m_window->draw(*m_rendererTiles[x][y]);
                }
            }
        }

        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));

        // Draw path
        rect.setFillColor(sf::Color(40, 90, 190));
        for (const auto& coords : m_path)
        {
            rect.setPosition({ (float)(coords.x * CELL_SIZE), (float)(coords.y * CELL_SIZE) });
            m_window->draw(rect);
        }

        // Draw start
        rect.setFillColor(sf::Color(240, 180, 10));
        rect.setPosition({ (float)(m_start.x * CELL_SIZE), (float)(m_start.y * CELL_SIZE) });
        m_window->draw(rect);

        // Draw goal
        rect.setFillColor(sf::Color(50, 200, 10));
        rect.setPosition({ (float)(m_goal.x * CELL_SIZE), (float)(m_goal.y * CELL_SIZE) });
        m_window->draw(rect);

        // Draw grid
        for (const auto& line : m_rendererGrid)
            m_window->draw(line);

    }

}
