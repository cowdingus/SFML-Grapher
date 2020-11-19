/*
 * ToDo:
 *   look at @setSize implementation
 *   fix bugs
 *
 * Bugs Found:
 *   crashes and hangs the whole computer when setting zoom value to some little value
 */

#include "CartesianGraphView.hpp"
#include "CartesianGrid.hpp"
#include "DotGraph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <iostream>
#include <cassert>

template<typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T>& vector)
{
	return out << "{" << vector.x << ", " << vector.y << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const sf::Rect<T>& rect)
{
	return out << "{" << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << "}" << std::endl;
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
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(800, 600), "NULL", sf::Style::Default, settings);
	window.setFramerateLimit(15);

	{
		std::cout << "Graph Tests" << std::endl;
		DotGraph lg;

		lg.addPoint({1, 1});
		assert(lg.getPoint(0) == sf::Vector2f(1, 1));
		lg.removePoint(0);
		assert(lg.getPointsCount() == 0);
		lg.addPoint({1, 1});
		assert(lg.getPoint(0) == sf::Vector2f(1, 1));
		lg.clearPoints();
		assert(lg.getPointsCount() == 0);
		std::cout << "Graph Data Insertion/Deletion Tests Passed" << std::endl;

		lg.setGridColor(sf::Color::White);
		assert(lg.getGridColor() == sf::Color::White);
		lg.setGridGap({10, 10});
		assert(lg.getGridGap() == sf::Vector2f(10, 10));

		lg.setColor(sf::Color::Black);
		assert(lg.getColor() == sf::Color::Black);

		CartesianGraphView view = lg.getView();
		view.setViewRect({0, 0, 10, 10});

		lg.setView(view);

		std::cout << lg.getView().getViewRect() << std::endl;
		assert(lg.getView().getViewRect() == sf::FloatRect(0, 0, 10, 10));

		std::cout << "Visual Properties Tests Passed" << std::endl;
	}

	{
		std::cout << "Graph View Tests" << std::endl;
		CartesianGraphView view;

		view.setCenter({100, 100});
		assert(view.getCenter() == sf::Vector2f(100, 100));
		view.setSize({100, 100});
		assert(view.getSize() == sf::Vector2f(100, 100));
		view.setViewRect({0, 0, 10, 10});
		assert(view.getViewRect() == sf::FloatRect(0, 0, 10, 10));
		assert(view.getCenter() == sf::Vector2f(5, 5));
		assert(view.getSize() == sf::Vector2f(10, 10));
		view.setCenter({10, 10});
		assert(view.getCenter() == sf::Vector2f(10, 10));
		view.setSize({10, 10});
		assert(view.getSize() == sf::Vector2f(10, 10));
		view.setZoom({10, 10});
		assert(view.getZoom() == sf::Vector2f(10, 10));

		view = CartesianGraphView();

		view.setCenter({1, 1});
		view.setSize({2, 2});

		std::cout << view.getTransform() * sf::Vector2f(0, 0) << std::endl;
		assert(view.getTransform() * sf::Vector2f(0, 0) == sf::Vector2f(0, 0));
	}

	std::cout << "All Tests Passed" << std::endl << std::endl;

	DotGraph lg({400, 400});
	lg.setPosition(100, 100);

	lg.addPoint({0, 0});
	lg.addPoint({-1, 0});
	lg.addPoint({1, 0});
	lg.addPoint({0, -1});
	lg.addPoint({0, 1});

	lg.addPoint({0, 2});
	lg.addPoint({-3, 0});

	lg.setGridGap({1, 1});
	lg.setGridColor(sf::Color(100, 100, 100));

	CartesianGraphView lgv;

	lgv.setCenter({-2, -2});
	lgv.setSize({8, 8});

	lgv.setViewRect({-2, -2, 8, 8});

	lgv.setCenter({0, 0});
	lgv.setSize({10, 10});

	lgv.setZoom({2,1});

	lg.setView(lgv);

	lg.setSize({200, 200});
	lg.setColor(sf::Color::Cyan);

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
				CartesianGraphView graphView = lg.getView();

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
					graphView.move({0, 0.1});
					break;
				case sf::Keyboard::Down:
					graphView.move({0, -0.1});
					break;
				case sf::Keyboard::Left:
					graphView.move({-0.1, 0});
					break;
				case sf::Keyboard::Right:
					graphView.move({0.1, 0});
					break;
				case sf::Keyboard::M:
					if (graphView.getZoom().x < 3 && graphView.getZoom().y < 3)
						graphView.setZoom(graphView.getZoom() + sf::Vector2f(0.1f, 0.1f));
					break;
				case sf::Keyboard::N:
					if (graphView.getZoom().x > 1 && graphView.getZoom().y > 1)
						graphView.setZoom(graphView.getZoom() - sf::Vector2f(0.1f, 0.1f));
					break;
				default:
					break;
				}

				lg.setView(graphView);

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
