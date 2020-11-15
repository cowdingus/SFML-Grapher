#include "CartesianGrid.hpp"
#include "CartesianGraphView.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <cmath>

void CartesianGrid::setSize(const sf::Vector2f& size)
{
	this->size = size;
	recalculateStretchTransform();
	needUpdate = true;
}

sf::Vector2f CartesianGrid::getSize() const
{
	return size;
}

void CartesianGrid::setView(const CartesianGraphView& view)
{
	this->view = view;
	recalculateStretchTransform();
	needUpdate = true;
}

void CartesianGrid::createHorizontalLine(float yPosition, const Span& xViewSpan)
{
	sf::Vector2f startPos, endPos;
	startPos = {xViewSpan.from, yPosition};
	endPos = {xViewSpan.to, yPosition};

	sf::Transform transform = stretchTransform.getTransform() * view.getInverseTransform();
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

	sf::Transform transform = stretchTransform.getTransform() * view.getInverseTransform();
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

	// calculate view area taking account of zooming, translation
	sf::FloatRect viewRegion = view.getViewRect();

	if ((viewRegion.width * viewRegion.height) == 0) return;

	createVerticalLines(viewRegion);
	createHorizontalLines(viewRegion);
}

CartesianGrid::CartesianGrid(sf::Vector2f gap, sf::FloatRect viewRegion)
	: gap(gap)
{
	view.setViewRect(viewRegion);
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
		needUpdate = false;
	}
}

void CartesianGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mesh, states);
}

void CartesianGrid::recalculateStretchTransform()
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
