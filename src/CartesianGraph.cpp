#include "CartesianGraph.hpp"
#include "CartesianCoordinateSystem.hpp"
#include "CartesianGrid.hpp"

CartesianGraph::CartesianGraph()
	: grid(coordinateSystem)
{

}

void CartesianGraph::setViewSize(const sf::Vector2f& size)
{
	viewRect.width = size.x;
	viewRect.height = size.y;
	grid.setViewRegion(viewRect);
	recalculateStretchTransform(static_cast<sf::Vector2f>(canvas.getSize()));
	grid.setStretchTransform(stretchTransform.getTransform());
	needUpdate = true;
}

sf::Vector2f CartesianGraph::getViewSize() const
{
	return {viewRect.width, viewRect.height};
}

void CartesianGraph::setSize(sf::Vector2f size)
{
	if (!canvas.create(static_cast<sf::Vector2u>(size).x, static_cast<sf::Vector2u>(size).y))
	{
		sf::err() << "Failed creating graph canvas!" << std::endl;
		return;
	}

	if (viewRect.width == 0 || viewRect.height == 0)
	{
		sf::err() << "Invalid view rect size" << std::endl;
		return;

		// Todo, generate default view instead of returning
	}

	// Instead of using view to zoom or translate, we use our own transform
	// to pervents stretching, that's why we set the view to the default size (canvas size).
	// take a look at #calculateStretchTransform(canvasSize) method
	sf::Vector2f canvasSize = static_cast<sf::Vector2f>(canvas.getSize());
	canvas.setView(sf::View({0, -canvasSize.y, canvasSize.x, canvasSize.y}));

	recalculateStretchTransform(canvasSize);
	grid.setStretchTransform(stretchTransform.getTransform());

	display.setTexture(canvas.getTexture(), true);
	needUpdate = true;
}

sf::Vector2u CartesianGraph::getSize() const
{
	return canvas.getSize();
}

void CartesianGraph::setZoom(sf::Vector2f zoom)
{
	viewTransform.setScale({1 / zoom.x, 1 / zoom.y});
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

void CartesianGraph::setUnitScaling(const sf::Vector2f &unitScaling)
{
	coordinateSystem.setScale(unitScaling);
	grid.update(true);
}

sf::Vector2f CartesianGraph::getUnitScaling() const
{
	return coordinateSystem.getScale();
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
	//sf::Transform transform =
	//    coordinateSystem.getTransform() * viewTransform.getInverseTransform() * stretchTransform.getTransform();
	sf::Transform transform =
	    coordinateSystem.getTransform() * stretchTransform.getTransform() * viewTransform.getInverseTransform();
	coords = transform.transformPoint(coords);
	return coords;
}

void CartesianGraph::recalculateStretchTransform(const sf::Vector2f& canvasViewSize)
{
	// To achieve desired effect (zooming / resizing without stretching its content)
	// we use this, a stretch transform.

	sf::Vector2f canvasNormalSize =
	    coordinateSystem.getInverseAxisTransform().transformPoint(canvasViewSize);

	sf::Vector2f stretchScale(
	    canvasNormalSize.x / viewRect.width,
	    -(canvasNormalSize.y / viewRect.height)
	);

	stretchTransform.setScale(stretchScale);
}

CartesianGraph::~CartesianGraph() {}
