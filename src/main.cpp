#include "CartesianCoordinateSystem.hpp"
#include "CartesianGrid.hpp"
#include "LineGraph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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
		std::cout << "CCS Tests" << std::endl;
		CartesianCoordinateSystem ccst;
		ccst.setPosition({50, 50});
		ccst.setScale({10, 10});

		assert(ccst.mapToLocalCoords({50, 50}) == sf::Vector2f(0, 0));
		assert(ccst.mapToWorldCoords({0, 0}) == sf::Vector2f(50, 50));
		assert(ccst.mapToLocalCoords({60, 40}) == sf::Vector2f({1, 1}));
		assert(ccst.mapToWorldCoords({1, 1}) == sf::Vector2f(60, 40));
		std::cout << "CCS Unit Conversion Test Passed" << std::endl;

		std::cout << ccst.getTransform() << std::endl;
		std::cout << ccst.getAxisTransform() << std::endl;

		assert(ccst.getAxisTransform() == sf::Transform().scale({10, -10}));
		assert(ccst.getTransform() == sf::Transform().translate(50, 50).scale({10, -10}));
		std::cout << "CCS Axis Transform Test Passed" << std::endl;
	}

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

	CartesianCoordinateSystem ccs;
	ccs.setPosition({50, 50});
	ccs.setScale({10, 10});

	std::cout << "CCS States" << std::endl
	          << "Position" << '\t' << ccs.getPosition() << std::endl
	          << "Scale" << '\t' << '\t' << ccs.getScale() << std::endl;

	std::cout << ccs.mapToLocalCoords({50, 50});
	std::cout << ccs.mapToWorldCoords({0, 0});

	sf::Transform t = window.getView().getTransform();
	std::cout << t.transformPoint(ccs.mapToWorldCoords({0, 0}));
	std::cout << t.transformPoint(ccs.mapToWorldCoords({5, 5}));
	std::cout << t.transformPoint(ccs.mapToWorldCoords({5, -5}));
	std::cout << t.transformPoint(ccs.mapToWorldCoords({-5, -5}));
	std::cout << t.transformPoint(ccs.mapToWorldCoords({-5, 5}));
	std::cout << std::endl;

	sf::VertexArray vertices(sf::Points, 5);
	vertices[0].position = ccs.mapToWorldCoords({0, 0});
	vertices[0].color = sf::Color::Blue;

	vertices[1].position = ccs.mapToWorldCoords({1, 1});
	vertices[2].position = ccs.mapToWorldCoords({1, -1});
	vertices[3].position = ccs.mapToWorldCoords({-1, -1});
	vertices[4].position = ccs.mapToWorldCoords({-1, 1});
	vertices[1].color = sf::Color::Red;
	vertices[2].color = sf::Color::Yellow;
	vertices[3].color = sf::Color::Yellow;
	vertices[4].color = sf::Color::Red;

	CartesianCoordinateSystem ccs_normal;
	ccs_normal.setPosition(50, 50);

	sf::VertexArray vertices_normal(sf::Points, 5);
	vertices_normal[0].position = ccs_normal.mapToWorldCoords({0, 0});
	vertices_normal[0].color = sf::Color::Blue;

	vertices_normal[1].position = ccs_normal.mapToWorldCoords({1, 1});
	vertices_normal[2].position = ccs_normal.mapToWorldCoords({1, -1});
	vertices_normal[3].position = ccs_normal.mapToWorldCoords({-1, -1});
	vertices_normal[4].position = ccs_normal.mapToWorldCoords({-1, 1});
	vertices_normal[1].color = sf::Color::Red;
	vertices_normal[2].color = sf::Color::Yellow;
	vertices_normal[3].color = sf::Color::Yellow;
	vertices_normal[4].color = sf::Color::Red;

	CartesianGrid cga(ccs);
	cga.setGap({1, 1});
	cga.setViewRegion({-5, -5, 10, 10});
	cga.setColor(sf::Color::White);

	CartesianGrid cg(ccs);
	cg.setGap({1, 1});
	cg.setViewRegion({0.75, 0.75, 2, 2});
	cg.setColor(sf::Color::Blue);

	LineGraph lg({400, 400});
	lg.setPosition(100, 100);

	lg.addPoint({0, 0});
	lg.addPoint({-1, 0});
	lg.addPoint({1, 0});
	lg.addPoint({0, -1});
	lg.addPoint({0, 1});

	lg.setZoom({2, 2});
	lg.setGridGap({2, 2});
	lg.setUnitScaling({10, 10});
	lg.setViewRegion({0, 0, 8, 8});
	lg.setSize({200, 200});
	lg.setGridColor(sf::Color(100, 100, 100));
	lg.setViewPosition({-2, -2});

	lg.setViewRegion({0, 0, 8, 8});
	lg.setViewPosition({-2, -2});


	sf::RectangleShape boundingBox;
	boundingBox.setSize(static_cast<sf::Vector2f>(lg.getSize()));
	boundingBox.setPosition({100, 100});
	boundingBox.setOutlineThickness(1);
	boundingBox.setOutlineColor(sf::Color::Magenta);
	boundingBox.setFillColor(sf::Color(0, 0, 0, 0));

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
				if (event.key.code == sf::Keyboard::W)
				{
					newView.move(0, -10);
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					newView.move(0, 10);
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					newView.move(-10, 0);
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					newView.move(10, 0);
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					lg.moveViewPosition({0, 0.1});
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					lg.moveViewPosition({0, -0.1});
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					lg.moveViewPosition({0.1, 0});
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					lg.moveViewPosition({-0.1, 0});
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
		window.draw(vertices);
		window.draw(vertices_normal);
		window.draw(cga);
		window.draw(cg);
		window.draw(lg);
		window.draw(boundingBox);
		window.display();
	}
}
