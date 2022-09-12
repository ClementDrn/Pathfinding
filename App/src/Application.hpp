#pragma once
#include <SFML/Application.hpp>

#include <SEL/Vec2.hpp>
#include <SEL/Reference.hpp>

#include <vector>
#include <array>


namespace App {

	#define	CELL_SIZE 30
	#define MAP_SIZE_X 32
	#define MAP_SIZE_Y 18
	
	class Application : public sf::Application
	{
	public:

		~Application() = default;

		static Application* create();
		static Application* get();
		static void destroy();

	private:

		Application();

		virtual void onEvent(const sf::Event& e);
		virtual void onUpdate(const sf::Time& ts);
		virtual void onRender();


		static Application* s_instance;
		

		// Events
		bool m_needsUpdate = true;
		bool m_isSpacePressed = false;
		bool m_isMouseLeftPressed = false;
		bool m_isMouseRightPressed = false;
		bool m_isObstacleMode = true;
		sf::Vector2i m_mousePosition;

		// Pathfinding
		sf::Vector2u m_start = { 8, 7 };
		sf::Vector2u m_goal = { 20, 10 };
		std::array<bool, MAP_SIZE_Y * MAP_SIZE_X> m_map;
		std::vector<sel::Vec2u> m_path;

		// Renderer
		std::array<sf::RectangleShape, MAP_SIZE_X + MAP_SIZE_Y> m_rendererGrid;
		std::array<std::array<sel::Ref<sf::RectangleShape>, MAP_SIZE_Y>, MAP_SIZE_X> m_rendererTiles;
	};

}
