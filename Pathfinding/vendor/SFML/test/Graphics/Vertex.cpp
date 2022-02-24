#include <SFML/Graphics/Vertex.hpp>
#include "GraphicsUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::Vertex class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::Vertex vertex;
            CHECK(vertex.position == sf::Vector2f(0.0f, 0.0f));
            CHECK(vertex.color == sf::Color(255, 255, 255));
            CHECK(vertex.texCoords == sf::Vector2f(0.0f, 0.0f));
        }

        SUBCASE("Position constructor")
        {
            const sf::Vertex vertex({1, 2});
            CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            CHECK(vertex.color == sf::Color(255, 255, 255));
            CHECK(vertex.texCoords == sf::Vector2f(0.0f, 0.0f));
        }

        SUBCASE("Position and color constructor")
        {
            const sf::Vertex vertex({1, 2}, {3, 4, 5, 6});
            CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            CHECK(vertex.color == sf::Color(3, 4, 5, 6));
            CHECK(vertex.texCoords == sf::Vector2f(0.0f, 0.0f));
        }

        SUBCASE("Position and coords constructor")
        {
            const sf::Vertex vertex({1, 2}, {3, 4});
            CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            CHECK(vertex.color == sf::Color(255, 255, 255));
            CHECK(vertex.texCoords == sf::Vector2f(3.0f, 4.0f));
        }

        SUBCASE("Position, color, and coords constructor")
        {
            const sf::Vertex vertex({1, 2}, {3, 4, 5, 6}, {7, 8});
            CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            CHECK(vertex.color == sf::Color(3, 4, 5, 6));
            CHECK(vertex.texCoords == sf::Vector2f(7.0f, 8.0f));
        }
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Vertex vertex({1, 2}, {3, 4, 5, 6}, {7, 8});
        static_assert(vertex.position == sf::Vector2f(1.0f, 2.0f));
        static_assert(vertex.color == sf::Color(3, 4, 5, 6));
        static_assert(vertex.texCoords == sf::Vector2f(7.0f, 8.0f));
    }
}
