#include "CartesianGrid.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <cmath>

void CartesianGrid::createHorizontalLine(float yPosition, const Span& xViewSpan)
{
	sf::Vector2f startPos, endPos;
	startPos = {xViewSpan.from, yPosition};
	endPos = {xViewSpan.to, yPosition};

	sf::Transform transform = ccs.getTransform() * viewTransform.getInverse() * stretchTransform;
	startPos = transform.transformPoint(startPos);
	endPos = transform.transformPoint(endPos);

	mesh.append({startPos, color});
	mesh.append({endPos, color});
}

void CartesianGrid::createVerticalLine(float xPosition, const Span& yViewSpan)
{
	sf::Vector2f startPos, endPos;
	startPos = {xPosition, yViewSpan.from};
	endPos = {xPosition, yViewSpan.to};

	sf::Transform transform = ccs.getTransform() * viewTransform.getInverse() * stretchTransform;
	startPos = transform.transformPoint(startPos);
	endPos = transform.transformPoint(endPos);

	mesh.append({startPos, color});
	mesh.append({endPos, color});
}

void CartesianGrid::createHorizontalLines(const sf::FloatRect& viewRegion)
{
	if (gap.y != 0)
	{
		Span yViewSpan = {viewRegion.top, viewRegion.top + viewRegion.height};
		Span xViewSpan = {viewRegion.left, viewRegion.left + viewRegion.width};
		const float yGap = gap.y;

		float startY = yViewSpan.from;
		startY = (std::floor(startY / yGap) + 1) * yGap;

		float endY = yViewSpan.to;

		for (float yPos = startY; yPos < endY; yPos += yGap)
		{
			createHorizontalLine(yPos, xViewSpan);
		}
	}
}

void CartesianGrid::createVerticalLines(const sf::FloatRect& viewRegion)
{
	if (gap.x != 0)
	{
		Span xViewSpan = {viewRegion.left, viewRegion.left + viewRegion.width};
		Span yViewSpan = {viewRegion.top, viewRegion.top + viewRegion.height};
		const float xGap = gap.x;

		float startX = xViewSpan.from;
		startX = (std::floor(startX / xGap) + 1) * xGap;

		float endX = xViewSpan.to;

		for (float xPos = startX; xPos < endX; xPos += xGap)
		{
			createVerticalLine(xPos, yViewSpan);
		}
	}
}

void CartesianGrid::updateGrid()
{
	mesh.clear();

	if ((viewRect.width * viewRect.height) == 0) return;

	// calculate view area taking account of zooming, translation
	sf::FloatRect viewRegion = viewTransform.transformRect(viewRect);

	createVerticalLines(viewRegion);
	createHorizontalLines(viewRegion);
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
	needUpdate = true;
}

const sf::FloatRect& CartesianGrid::getViewRegion() const
{
	return viewRect;
}
void CartesianGrid::setViewRegion(const sf::FloatRect& viewRegion)
{
	this->viewRect = viewRegion;
	needUpdate = true;
}
void CartesianGrid::setViewRegion(const sf::Vector2f& topLeftPosition, const sf::Vector2f& size)
{
	this->viewRect = {topLeftPosition.x, topLeftPosition.y, size.x, size.y};
	needUpdate = true;
}
void CartesianGrid::moveViewRegion(const sf::Vector2f& offset)
{
	viewTransform.translate(offset);
	needUpdate = true;
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

void CartesianGrid::update(bool force)
{
	if (needUpdate || force)
	{
		updateGrid();
	}
}

void CartesianGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mesh, states);
}

void CartesianGrid::setViewTransform(const sf::Transform& transform)
{
	viewTransform = transform;
	needUpdate = true;
}
void CartesianGrid::setStretchTransform(const sf::Transform& transform)
{
	stretchTransform = transform;
	needUpdate = true;
}
