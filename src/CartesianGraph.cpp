#include "CartesianGraph.hpp"
#include "CartesianGraphView.hpp"
#include "CartesianGrid.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <exception>

CartesianGraph::CartesianGraph()
{

}

void CartesianGraph::setView(const CartesianGraphView& view)
{
	this->view = view;
	grid.setView(view);
	recalculateStretchTransform();
	needUpdate = true;
}

const CartesianGraphView& CartesianGraph::getView() const
{
	return view;
}

void CartesianGraph::setSize(sf::Vector2f size)
{
	this->size = size;
	grid.setSize(size);

	sf::FloatRect viewRect = view.getViewRect();

	if (viewRect.width == 0 || viewRect.height == 0)
	{
		sf::err() << "Invalid view rect size, using default rect size" << std::endl;

		// Todo, generate default view instead of returning
		view.setViewRect({0, 0, size.x, size.y});
	}

	recalculateStretchTransform();

	needUpdate = true;
}

sf::Vector2f CartesianGraph::getSize() const
{
	return size;
}

void CartesianGraph::setGridGap(const sf::Vector2f& gap)
{
	grid.setGap(gap);
}

sf::Vector2f CartesianGraph::getGridGap() const
{
	return grid.getGap();
}

void CartesianGraph::setGridColor(sf::Color color)
{
	grid.setColor(color);
}

sf::Color CartesianGraph::getGridColor() const
{
	return grid.getColor();
}

void CartesianGraph::update()
{
	if (needUpdate)
	{
		needUpdate = false;
	}
	grid.update();
	updateGraph();
}

sf::Vector2f CartesianGraph::getPointPosition(sf::Vector2f coords) const
{
	sf::Transform transform = stretchTransform.getTransform() * view.getInverseTransform();
	coords = transform.transformPoint(coords);
	return coords;
}

void CartesianGraph::recalculateStretchTransform()
{
	// To achieve desired effect (zooming / resizing without stretching its content)
	// we use this, a stretch transform.

	sf::FloatRect viewRect = view.getViewRect();

	sf::Vector2f stretchScale(
	    size.x / viewRect.width,
	    -(size.y / viewRect.height)
	);

	stretchTransform.setScale(stretchScale);
}

CartesianGraph::~CartesianGraph() {}
