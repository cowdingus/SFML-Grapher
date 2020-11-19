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
	points.push_back(point);
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

void LineGraph::createLine(const sf::Vector2f& from, const sf::Vector2f& to)
{
	
}

void LineGraph::updateContent()
{

}

void LineGraph::render(sf::RenderTexture& canvas) const
{
	canvas.clear();
	canvas.draw(mesh);
	canvas.draw(grid);
	canvas.display();
}
