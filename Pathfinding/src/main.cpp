#include <SFML/Graphics.hpp>
#include "PathFinding.h"


using namespace std;
using namespace sf;


void displayLines(Vector2u& winSize, RenderWindow& window, const int zoom)
{
    for (int i = 0; i < winSize.x; i += zoom)
    {
        RectangleShape line(Vector2f(1, winSize.y));
        line.setFillColor(Color::Black);
        line.setPosition(Vector2f(i, 0));
        window.draw(line);
    }
    for (int i = 0; i < winSize.y; i += zoom)
    {
        RectangleShape line(Vector2f(winSize.x, 1));
        line.setFillColor(Color::Black);
        line.setPosition(Vector2f(0, i));
        window.draw(line);
    }
}

void displayObstacles(Vector2u& winSize, RenderWindow& window, vector<string>& labyrinth, const int zoom)
{
    for (int i = 0; i < labyrinth.size(); i++)
    {
        for (int j = 0; j < labyrinth[i].size(); j++)
        {
            if (labyrinth[i][j] == '#')
            {
                RectangleShape square(Vector2f(zoom, zoom));
                square.setFillColor(Color(190, 24, 74));
                square.setPosition(Vector2f(j* zoom, i* zoom));
                window.draw(square);
            }
        }
    }
}

void displayStart_Target(Vector2u& winSize, RenderWindow& window, Position2D start, Position2D target, const int zoom)
{
    RectangleShape rect1(Vector2f(zoom, zoom)), rect2(Vector2f(zoom, zoom));
    rect1.setFillColor(Color(25, 127, 48));
    rect2.setFillColor(Color(232, 158, 0));
    rect1.setPosition(Vector2f(start.x * zoom, start.y * zoom));
    rect2.setPosition(Vector2f(target.x * zoom, target.y * zoom));
    window.draw(rect1);
    window.draw(rect2);

}

void displayPath(Vector2u& winSize, RenderWindow& window, vector<Position2D> path, const int zoom)
{
    for (auto& const coos : path)
    {
        RectangleShape square(Vector2f(zoom, zoom));
        square.setFillColor(Color(16, 107, 189));
        square.setPosition(Vector2f(coos.x * zoom, coos.y * zoom));
        window.draw(square);
    }
}


int main()
{
    // Window constructor
    RenderWindow window(VideoMode(1500, 900), "Path Finding A*", Style::Close);
    Vector2u winSize(window.getSize());
    int zoom(30);

    // Event variables
    bool right(false), left(false), isObstaclesSelected(true), update(true);
    bool space(false);
    Position2D mouse = { 0, 0 };

    // Labyrinth
    vector<string> labyrinth;
    string st;
    for (int i = 0; i < winSize.x / zoom; i++) {
        st.push_back('#');
    }
    labyrinth.push_back(st);

    for (int i = 0; i < winSize.y / zoom - 2; i++) {
        st = "";
        st.push_back('#');
        for (int j = 0; j < winSize.x / zoom - 2; j++) {
            st.push_back('.');
        }
        st.push_back('#');
        labyrinth.push_back(st);
    }

    st = "";
    for (int i = 0; i < winSize.x / zoom; i++) {
        st.push_back('#');
    }
    labyrinth.push_back(st);


    // Other Path Algorithm dependencies
    Position2D start = { 8, 2 };
    Position2D target = { 3, 6 };
    vector<Position2D> path;

    // Debug dependencies
    Font font;
    if (!font.loadFromFile("ARIAL.TTF"))
    {
        cout << "Error: The font couldn't be loaded.\n";
    }

    // Main Loop
    while (window.isOpen())
    {
        // Events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::LControl)
                    isObstaclesSelected = false;
                if (event.key.code == sf::Keyboard::Space)
                    space = true;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::LControl)
                    isObstaclesSelected = true;
                if (event.key.code == sf::Keyboard::Space)
                    space = false;
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                    right = true;
                if (event.mouseButton.button == sf::Mouse::Left)
                    left = true;
                mouse.x = event.mouseButton.x;
                mouse.y = event.mouseButton.y;
                update = true;
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                    right = false;
                if (event.mouseButton.button == sf::Mouse::Left)
                    left = false;
            }

            if (event.type == sf::Event::MouseMoved)
            {
                mouse.x = event.mouseMove.x;
                mouse.y = event.mouseMove.y;
                if (left || right)
                    update = true;
            }
        }


        if (update)
        {
            int x(mouse.x / zoom), y(mouse.y / zoom);
            if (x < labyrinth[0].size() && x >= 0 && y < labyrinth.size() && y >= 0)
            {
                if (isObstaclesSelected) {
                    if ((x != start.x || y != start.y) && (x != target.x || y != target.y))
                    {
                        if (left)
                            labyrinth[y][x] = '#';
                        if (right)
                            labyrinth[y][x] = '.';
                    }
                }
                else {
                    if (left) {
                        start.x = x;
                        start.y = y;
                    }
                    if (right) {
                        target.x = x;
                        target.y = y;
                    }
                    labyrinth[y][x] = '.';
                }
                //path = findPath(labyrinth, start, target);
                if (space)
                    path = findPath(labyrinth, start, target, winSize, window, zoom, font, true);
                else
                    path = findPath(labyrinth, start, target, winSize, window, zoom, font, false);

                update = false;
            }
        }

        // Clear Window
        window.clear(Color::White);

        displayObstacles(winSize, window, labyrinth, zoom);
        displayPath(winSize, window, path, zoom);
        displayStart_Target(winSize, window, start, target, zoom);
        displayLines(winSize, window, zoom);

        // Display Window
        window.display();
    }

    return 0;
}