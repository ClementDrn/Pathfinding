#pragma once

#include <SFML/Graphics.hpp>

#include <string>


namespace sf {

	class Application
	{
	public:

		struct Properties
		{
			uint32_t winWidth	= 960;
			uint32_t winHeight	= 540;
			int style			= Style::Close;		// sf::Style
			Color clearColor	= Color::Black;
		};

		~Application();

		void run();

	protected:

		Application(const std::string& title, const Properties& props = {});

		virtual void onEvent(const Event& e) = 0;
		virtual void onUpdate(const Time& ts) = 0;
		virtual void onRender() = 0;


		RenderWindow* m_window;
		Color m_clearColor;
	};

}
