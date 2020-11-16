#include "DotGraph.hpp"
#include "CartesianGraph.hpp"

#include <SFML/System/Vector2.hpp>

DotGraph::DotGraph()
{
}

DotGraph::DotGraph(sf::Vector2f size)
{
	view.setSize(size);
	setSize(size);
}

void DotGraph::setSize(const sf::Vector2f& size)
{
	if (!canvas.create(size.x, size.y))
	{
		sf::err() << "Can't create graph canvas!" << std::endl;
	}
	else
	{
		canvas.setView(sf::View({0, -size.y, size.x, size.y}));
		display.setTexture(canvas.getTexture(), true);

		CartesianGraph::setSize(size);
	}
}

sf::Vector2f DotGraph::getPoint(std::size_t index) const
{
	return graphPoints[index];
}

void DotGraph::addPoint(sf::Vector2f datum)
{
	graphPoints.emplace_back(datum.x, datum.y);
	needUpdate = true;
}

void DotGraph::removePoint(std::size_t index)
{
	graphPoints.erase(graphPoints.begin() + index);
	needUpdate = true;
}

void DotGraph::clearPoints()
{
	graphPoints.clear();
	needUpdate = true;
}

std::size_t DotGraph::getPointsCount() const
{
	return graphPoints.size();
}

void DotGraph::updateGraph()
{
	if (needUpdate)
	{
		updateContent();
	}

	updateCanvas();
}

void DotGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(display, states);
}

void DotGraph::createPoint(const sf::Vector2f& coords)
{
	auto newDot = dot;

	// Get the point's position on the graph, taking account of
	// zooming/translate/rotate transform
	sf::Vector2f pointPosition = getPointPosition(coords);

	// Because the dot is so small, we scale it by 9 so it can be seen
	// then we move the dot to the point's position
	sf::Transform dotTransform;
	dotTransform.translate(pointPosition);
	dotTransform.scale({9, 9});

	// Transform the dot to the desired coords
	for (auto& vertx : newDot)
	{
		vertx.position = dotTransform.transformPoint(vertx.position);

		mesh.append(vertx);
	}
}

void DotGraph::updateContent()
{
	mesh.clear();

	// Determine how big we can see taking account of the zooming and translation
	sf::FloatRect viewRegion = view.getViewRect();

	for (auto& datum : graphPoints)
	{
		Span xViewSpan {viewRegion.left, viewRegion.left + viewRegion.width};
		Span yViewSpan {viewRegion.top, viewRegion.top + viewRegion.height};

		// If not in sight (not visible), ignore it
		if (datum.x > xViewSpan.to || datum.x < xViewSpan.from ||
		        datum.y > yViewSpan.to || datum.y < yViewSpan.from)
			continue;

		// If in sight (visible), draw it
		createPoint(datum);
	}
}

void DotGraph::updateCanvas()
{
	canvas.clear();
	canvas.draw(grid);
	canvas.draw(mesh);
	canvas.display();
}
