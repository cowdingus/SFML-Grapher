#include "DotGraph.hpp"
#include "CartesianGraph.hpp"

#include <SFML/System/Vector2.hpp>

DotGraph::DotGraph()
{
}

DotGraph::DotGraph(sf::Vector2f size)
{
	setSize(size);
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

void DotGraph::setColor(sf::Color color)
{
	dotColor = color;

	for (std::size_t i; i < mesh.getVertexCount(); ++i)
	{
		mesh[i].color = dotColor;
	}
}

sf::Color DotGraph::getColor() const
{
	return dotColor;
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
	for (auto& vertex : newDot)
	{
		vertex.position = dotTransform.transformPoint(vertex.position);
		vertex.color = dotColor;

		mesh.append(vertex);
	}
}

void DotGraph::updateContent()
{
	mesh.clear();

	sf::FloatRect viewRect = view.getViewRect();

	for (const auto& datum : graphPoints)
	{
		// If not in sight (not visible), ignore it
		if (!viewRect.contains(datum))
			continue;

		// If in sight (visible), draw it
		createPoint(datum);
	}
}

void DotGraph::render(sf::RenderTexture& canvas) const
{
	canvas.clear();
	canvas.draw(grid);
	canvas.draw(mesh);
	canvas.display();
}
