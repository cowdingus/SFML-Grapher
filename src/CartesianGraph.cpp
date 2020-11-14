#include "CartesianGraph.hpp"
#include "CartesianGrid.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <exception>

CartesianGraph::CartesianGraph()
{

}

void CartesianGraph::setViewSize(const sf::Vector2f& viewSize)
{
	viewRect.width = viewSize.x;
	viewRect.height = viewSize.y;
	viewTransform.setOrigin(viewSize.x / 2, viewSize.y / 2);
	grid.setViewRegion(viewRect);
	recalculateStretchTransform(size);
	grid.setStretchTransform(stretchTransform.getTransform());
	needUpdate = true;
}

sf::Vector2f CartesianGraph::getViewSize() const
{
	return {viewRect.width, viewRect.height};
}

void CartesianGraph::setSize(sf::Vector2f size)
{
	this->size = size;

	if (viewRect.width == 0 || viewRect.height == 0)
	{
		sf::err() << "Invalid view rect size" << std::endl;
		return;

		// Todo, generate default view instead of returning
	}

	recalculateStretchTransform(size);
	grid.setStretchTransform(stretchTransform.getTransform());

	needUpdate = true;
}

sf::Vector2f CartesianGraph::getSize() const
{
	return size;
}

void CartesianGraph::setZoom(sf::Vector2f zoom)
{
	if (zoom.x == 0 || zoom.y == 0) throw "Division by 0 error";
	viewTransform.setScale({1 / zoom.x, 1 / zoom.y});
	if (viewTransform.getScale().x == 0.f || viewTransform.getScale().y == 0.f) throw "Division by non-0 resulted in 0";
	grid.setViewTransform(viewTransform.getTransform());
	needUpdate = true;
}

sf::Vector2f CartesianGraph::getZoom() const
{
	return {1 / viewTransform.getScale().x, 1 / viewTransform.getScale().y};
}

void CartesianGraph::setViewRect(const sf::FloatRect& rect)
{
	setViewPosition({rect.left, rect.top});
	setViewSize({rect.width, rect.height});
}

sf::FloatRect CartesianGraph::getViewRect() const
{
	auto viewPosition = getViewPosition();
	auto viewSize = getViewSize();
	return {viewPosition.x, viewPosition.y, viewSize.x, viewSize.y};
}

void CartesianGraph::setViewPosition(const sf::Vector2f& position)
{
	viewTransform.setPosition(position);
	grid.setViewTransform(viewTransform.getTransform());
	needUpdate = true;
}

sf::Vector2f CartesianGraph::getViewPosition() const
{
	return viewTransform.getPosition();
}

void CartesianGraph::moveViewPosition(const sf::Vector2f& offset)
{
	viewTransform.move(offset);
	grid.setViewTransform(viewTransform.getTransform());
	needUpdate = true;
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
	sf::Transform transform = stretchTransform.getTransform() * viewTransform.getInverseTransform();
	coords = transform.transformPoint(coords);
	return coords;
}

void CartesianGraph::recalculateStretchTransform(const sf::Vector2f& canvasViewSize)
{
	// To achieve desired effect (zooming / resizing without stretching its content)
	// we use this, a stretch transform.

	sf::Vector2f stretchScale(
	    canvasViewSize.x / viewRect.width,
	    -(canvasViewSize.y / viewRect.height)
	);

	stretchTransform.setScale(stretchScale);
}

CartesianGraph::~CartesianGraph() {}
