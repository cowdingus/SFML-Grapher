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
}

const CartesianGraphView& CartesianGraph::getView() const
{
	return view;
}

void CartesianGraph::setSize(sf::Vector2f size)
{
	if (!canvas.create(size.x, size.y))
	{
		throw "Can't create graph canvas!";
	}
	else
	{
		canvas.setView(sf::View({0, -size.y, size.x, size.y}));
		display.setTexture(canvas.getTexture(), true);
		display.setPosition(size.x / 2, size.y / 2);

		this->size = size;
		grid.setSize(size);

		sf::FloatRect viewRect = view.getViewRect();

		// If view rect are not set, set default
		if (viewRect.width == 0 || viewRect.height == 0)
		{
			view.setViewRect({0, 0, size.x, size.y});
		}

		recalculateStretchTransform();
	}
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
	grid.update();
	updateContent();	
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

void CartesianGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	render(canvas);
	target.draw(display, states);
}

CartesianGraph::~CartesianGraph() {}
