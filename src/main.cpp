#include "CartesianGrid.hpp"
#include "LineGraph.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>

template<typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T>& vector)
{
	return out << "{" << vector.x << ", " << vector.y << "}";
}

std::ostream& operator<<(std::ostream& out, const sf::Transform& transform)
{
	const float* m = transform.getMatrix();

	out << "[" << m[0] << '\t' << m[4] << '\t' << m[8 ] << '\t' << m[12] << '\t' << "]" << std::endl
	    << "[" << m[1] << '\t' << m[5] << '\t' << m[9 ] << '\t' << m[13] << '\t' << "]" << std::endl
	    << "[" << m[2] << '\t' << m[6] << '\t' << m[10] << '\t' << m[14] << '\t' << "]" << std::endl
	    << "[" << m[3] << '\t' << m[7] << '\t' << m[11] << '\t' << m[15] << '\t' << "]" << std::endl;

	return out;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "NULL");
	window.setFramerateLimit(15);
	window.setVerticalSyncEnabled(true);

	{
		std::cout << "Graph Tests" << std::endl;
		LineGraph lg;

		lg.addPoint({1, 1});
		assert(lg.getPoint(0) == sf::Vector2f(1, 1));
		lg.removePoint(0);
		assert(lg.getPointsCount() == 0);
		lg.addPoint({1, 1});
		assert(lg.getPoint(0) == sf::Vector2f(1, 1));
		lg.clearPoints();
		assert(lg.getPointsCount() == 0);
		std::cout << "Graph Data Insertion/Deletion Test Passed" << std::endl;
	}

	std::cout << "All Tests Passed" << std::endl << std::endl;

	LineGraph lg({400, 400});
	lg.setPosition(100, 100);

	lg.addPoint({0, 0});
	lg.addPoint({-1, 0});
	lg.addPoint({1, 0});
	lg.addPoint({0, -1});
	lg.addPoint({0, 1});

	lg.setZoom({2, 1});
	lg.setGridGap({1, 1});
	lg.setSize({200, 200});
	lg.setGridColor(sf::Color(100, 100, 100));

	lg.setViewPosition({-2, -2});
	lg.setViewSize({8, 8});

	lg.setViewRect({-2, -2, 8, 8});


	sf::RectangleShape boundingBox;
	boundingBox.setSize(static_cast<sf::Vector2f>(lg.getSize()));
	boundingBox.setPosition({100, 100});
	boundingBox.setOutlineThickness(1);
	boundingBox.setOutlineColor(sf::Color::Magenta);
	boundingBox.setFillColor(sf::Color(0, 0, 0, 0));

	sf::Font arial;
	if (!arial.loadFromFile("debug/Arial.ttf"))
	{
		sf::err() << "Error: Can't load font file" << std::endl;
	}

	sf::Text keymapGuide;
	keymapGuide.setString(
	    "[M] to zoom in\n"
	    "[N] to zoom out\n"
	    "[Arrow keys] to move graph\n"
	    "[W | A | S | D] to move view\n"
	);
	keymapGuide.setFont(arial);
	keymapGuide.setCharacterSize(12);
	keymapGuide.setFillColor(sf::Color::White);

	keymapGuide.setPosition(
	{
		window.getSize().x - keymapGuide.getGlobalBounds().width,
		window.getSize().y - keymapGuide.getGlobalBounds().height
	}
	);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				sf::View newView = window.getView();

				switch (event.key.code)
				{
				case sf::Keyboard::W:
					newView.move(0, -10);
					break;
				case sf::Keyboard::S:
					newView.move(0, 10);
					break;
				case sf::Keyboard::A:
					newView.move(-10, 0);
					break;
				case sf::Keyboard::D:
					newView.move(10, 0);
					break;
				case sf::Keyboard::Up:
					lg.moveViewPosition({0, 0.1});
					break;
				case sf::Keyboard::Down:
					lg.moveViewPosition({0, -0.1});
					break;
				case sf::Keyboard::Left:
					lg.moveViewPosition({-0.1, 0});
					break;
				case sf::Keyboard::Right:
					lg.moveViewPosition({0.1, 0});
					break;
				case sf::Keyboard::M:
					if (lg.getZoom().x < 3 || lg.getZoom().y < 3) lg.setZoom(lg.getZoom() + sf::Vector2f(0.1f, 0.1f));
					break;
				case sf::Keyboard::N:
					if (lg.getZoom().x > 1 || lg.getZoom().y > 1) lg.setZoom(lg.getZoom() - sf::Vector2f(0.1f, 0.1f));
					break;
				default:
					break;
				}

				window.setView(newView);
				break;
			}
			default:
			{
				break;
			}
			}
		}

		lg.update();

		window.clear();
		window.draw(lg);
		window.draw(boundingBox);
		window.draw(keymapGuide);
		window.display();
	}
}
