#include "LineGraph.hpp"

LineGraph::LineGraph()
{
}

LineGraph::LineGraph(const sf::Vector2f& size)
{
	setSize(size);
}

std::size_t LineGraph::getPointsCount() const
{
	return points.size();
}

sf::Vector2f LineGraph::getPoint(std::size_t index) const
{
	return points.at(index);
}

void LineGraph::addPoint(const sf::Vector2f &point)
{
	points.emplace_back(point);
	needUpdate = true;
}

void LineGraph::removePoint(std::size_t index)
{
	points.erase(points.begin() + index);
	needUpdate = true;
}

void LineGraph::clearPoints()
{
	points.clear();
	needUpdate = true;
}

void LineGraph::setLineThickness(float thickness)
{
	lineThickness = thickness;
	needUpdate = true;
}

float LineGraph::getLineThickness() const
{
	return lineThickness;
}

void LineGraph::setLineColor(sf::Color color)
{
	lineColor = color;
	for (std::size_t i = 0; i < mesh.getVertexCount(); ++i)
	{
		mesh[i].color = lineColor;
	}
}

sf::Color LineGraph::getLineColor() const
{
	return lineColor;
}

std::array<sf::Vector2f, 2> LineGraph::createLineSegment(const sf::Vector2f& point)
{
	std::array<sf::Vector2f, 2> segment;

	segment[0] = {point.x, point.y + lineThickness / 2};
	segment[1] = {point.x, point.y - lineThickness / 2};

	return segment;
}

void LineGraph::updateContent()
{
	mesh.clear();
	
	sf::FloatRect viewRect = view.getViewRect();

	for (const auto& point : points)
	{
	//	if (!viewRect.contains(point))
	//		continue;

		sf::Vector2f pos = getPointPosition(point);

		auto segment = createLineSegment(pos);
		mesh.append({segment[0], lineColor});
		mesh.append({segment[1], lineColor});
	}
}

void LineGraph::render(sf::RenderTexture& canvas) const
{
	canvas.clear();
	canvas.draw(grid);
	canvas.draw(mesh);
	canvas.display();
}
