#include "LineGraph.hpp"
#include "CartesianGrid.hpp"
#include <SFML/System/Vector2.hpp>

void LineGraph::createDot(sf::Vector2f coords)
{
	auto newDot = dot;

	// Determine dot position
	sf::Transform transform =
	    coordinateSystem.getTransform() * viewTransform.getInverseTransform() * stretchTransform.getTransform();
	coords = transform.transformPoint(coords);

	sf::Transform dotTransform;
	dotTransform.translate(coords);
	dotTransform.scale({9, 9});

	// Transform the dot to the desired coords
	for (auto& vertx : newDot)
	{
		vertx.position = dotTransform.transformPoint(vertx.position);

		mesh.append(vertx);
	}
}

void LineGraph::calculateStretchTransform(const sf::Vector2f& canvasViewSize)
{
	sf::Vector2f canvasNormalSize =
	    coordinateSystem.getInverseAxisTransform().transformPoint(canvasViewSize);

	sf::Vector2f stretchScale(
	    canvasNormalSize.x / viewRect.width,
	    -(canvasNormalSize.y / viewRect.height)
	);

	stretchTransform.setScale(stretchScale);
}

void LineGraph::updateGraph()
{
	mesh.clear();

	// Determine how big we can see taking account of the zooming and translation
	sf::Transform transform = viewTransform.getTransform();
	sf::FloatRect viewRegion = transform.transformRect(viewRect);

	for (auto& datum : graphPoints)
	{
		Span xViewSpan {viewRegion.left, viewRegion.left + viewRegion.width};
		Span yViewSpan {viewRegion.top, viewRegion.top + viewRegion.height};

		// If not in sight (not visible), ignore it
		if (datum.x > xViewSpan.to || datum.x < xViewSpan.from ||
		        datum.y > yViewSpan.to || datum.y < yViewSpan.from)
			continue;

		// If in sight (visible), draw it
		createDot(datum);
	}

	canvas.clear();
	canvas.draw(grid);
	canvas.draw(mesh);
	canvas.display();
}

LineGraph::LineGraph()
	: grid(coordinateSystem)
{
}

LineGraph::LineGraph(sf::Vector2f size)
	: grid(coordinateSystem)
{
	setViewRegion({0, 0, size.x, size.y});
	setSize(size);
}

sf::Vector2f LineGraph::getPoint(std::size_t index) const
{
	return graphPoints[index];
}

void LineGraph::addPoint(sf::Vector2f datum)
{
	graphPoints.emplace_back(datum.x, datum.y);
	needUpdate = true;
}

void LineGraph::removePoint(std::size_t index)
{
	graphPoints.erase(graphPoints.begin() + index);
	needUpdate = true;
}

void LineGraph::clearPoints()
{
	graphPoints.clear();
	needUpdate = true;
}

std::size_t LineGraph::getPointsCount() const
{
	return graphPoints.size();
}

// View region, Not to be confused with size of graph
sf::FloatRect LineGraph::getViewRegion() const
{
	return viewRect;
}

void LineGraph::setViewRegion(sf::FloatRect viewRegion)
{
	viewRect = viewRegion;
	grid.setViewRegion(viewRegion);
	needUpdate = true;
}

// Size of the graph
sf::Vector2u LineGraph::getSize() const
{
	return canvas.getSize();
}

void LineGraph::setSize(sf::Vector2f size)
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
	// to pervents stretching. We set the view to the canvas' size.
	sf::Vector2f canvasSize = static_cast<sf::Vector2f>(canvas.getSize());
	canvas.setView(sf::View({0, -canvasSize.y, canvasSize.x, canvasSize.y}));

	calculateStretchTransform(canvasSize);
	grid.setStretchTransform(stretchTransform.getTransform());

	display.setTexture(canvas.getTexture(), true);
	needUpdate = true;
}

sf::Vector2f LineGraph::getZoom() const
{
	return viewTransform.getScale();
}

void LineGraph::setZoom(sf::Vector2f zoom)
{
	viewTransform.setScale({1 / zoom.x, 1 / zoom.y});
	grid.setViewTransform(viewTransform.getTransform());
	needUpdate = true;
}

sf::Vector2f LineGraph::getViewPosition() const
{
	return viewTransform.getPosition();
}

void LineGraph::setViewPosition(const sf::Vector2f& position)
{
	viewTransform.setPosition(position);
	grid.setViewRegion(viewRect);
	needUpdate = true;
}

void LineGraph::moveViewPosition(const sf::Vector2f& offset)
{
	viewTransform.move(offset);
	grid.moveViewRegion(offset);
	needUpdate = true;
}

void LineGraph::update()
{
	grid.update();
	if (needUpdate)
	{
		updateGraph();
		needUpdate = false;
	}
}

void LineGraph::setGridGap(const sf::Vector2f& gap)
{
	grid.setGap(gap);
}

sf::Vector2f LineGraph::getGridGap() const
{
	return grid.getGap();
}

void LineGraph::setGridColor(sf::Color color)
{
	grid.setColor(color);
}

sf::Color LineGraph::getGridColor() const
{
	return grid.getColor();
}

void LineGraph::setUnitScaling(const sf::Vector2f &unitScaling)
{
	coordinateSystem.setScale(unitScaling);
	grid.update(true);
}

sf::Vector2f LineGraph::getUnitScaling() const
{
	return coordinateSystem.getScale();
}

void LineGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(display, states);
}
