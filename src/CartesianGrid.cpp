#include "CartesianGrid.hpp"

#include <cmath>

void CartesianGrid::createHorizontalLine(float yPosition)
{
	sf::FloatRect viewRegion = viewTransform.transformRect(viewRect);

	sf::Vector2f startPos, endPos;
	startPos = {viewRegion.left, yPosition};
	endPos = {viewRegion.left + viewRegion.width, yPosition};

	sf::Transform transform = ccs.getTransform() * viewTransform.getInverse() * stretchTransform;
	startPos = transform.transformPoint(startPos);
	endPos = transform.transformPoint(endPos);

	mesh.append({startPos, color});
	mesh.append({endPos, color});
}

void CartesianGrid::createVerticalLine(float xPosition)
{
	sf::FloatRect viewRegion = viewTransform.transformRect(viewRect);

	sf::Vector2f startPos, endPos;
	startPos = {xPosition, viewRegion.top};
	endPos = {xPosition, viewRegion.top + viewRegion.height};

	sf::Transform transform = ccs.getTransform() * viewTransform.getInverse() * stretchTransform;
	startPos = transform.transformPoint(startPos);
	endPos = transform.transformPoint(endPos);

	mesh.append({startPos, color});
	mesh.append({endPos, color});
}

void CartesianGrid::updateGrid()
{
	mesh.clear();

	if ((viewRect.width * viewRect.height) == 0) return;

	sf::FloatRect viewRegion = viewTransform.transformRect(viewRect);

	// X / Vertical Lines Creation
	if (gap.x != 0)
	{
		Span xViewSpan = {viewRegion.left, viewRegion.left + viewRegion.width};
		float xGap = gap.x;

		float startX = xViewSpan.from;
		startX = (std::floor(startX / xGap) + 1) * xGap;

		float endX = xViewSpan.to;

		for (float xPos = startX; xPos < endX; xPos += xGap)
		{
			createVerticalLine(xPos);
		}
	}

	// Y / Horizontal Lines Creation
	if (gap.y != 0)
	{
		Span yViewSpan = {viewRegion.top, viewRegion.top + viewRegion.height};
		float yGap = gap.y;

		float startY = yViewSpan.from;
		startY = (std::floor(startY / yGap) + 1) * yGap;

		float endY = yViewSpan.to;

		for (float yPos = startY; yPos < endY; yPos += yGap)
		{
			createHorizontalLine(yPos);
		}
	}
}

CartesianGrid::CartesianGrid(CartesianCoordinateSystem& ccs, sf::Vector2f gap, sf::FloatRect viewRegion)
	: ccs(ccs), gap(gap), viewRect(viewRegion)
{

}

sf::Vector2f CartesianGrid::getGap() const
{
	return gap;
}
void CartesianGrid::setGap(const sf::Vector2f& gap)
{
	this->gap = gap;
	updateGrid();
}

const sf::FloatRect& CartesianGrid::getViewRegion() const
{
	return viewRect;
}
void CartesianGrid::setViewRegion(const sf::FloatRect& viewRegion)
{
	this->viewRect = viewRegion;
	updateGrid();
}
void CartesianGrid::setViewRegion(const sf::Vector2f& topLeftPosition, const sf::Vector2f& size)
{
	this->viewRect = {topLeftPosition.x, topLeftPosition.y, size.x, size.y};
	updateGrid();
}
void CartesianGrid::moveViewRegion(const sf::Vector2f& offset)
{
	viewTransform.translate(offset);
	updateGrid();
}

sf::Color CartesianGrid::getColor() const
{
	return color;
}
void CartesianGrid::setColor(sf::Color color)
{
	this->color = color;
	for (std::size_t index = 0; index < mesh.getVertexCount(); ++index)
	{
		mesh[index].color = color;
	}
}

void CartesianGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mesh, states);
}

void CartesianGrid::setViewTransform(const sf::Transform& transform)
{
	viewTransform = transform;
}
void CartesianGrid::setStretchTransform(const sf::Transform& transform)
{
	stretchTransform = transform;
}
