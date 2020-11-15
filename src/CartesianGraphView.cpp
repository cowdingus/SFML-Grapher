#include "CartesianGraphView.hpp"
#include <SFML/Graphics/Transform.hpp>

CartesianGraphView::CartesianGraphView()
{

}

void CartesianGraphView::setSize(const sf::Vector2f& viewSize)
{
	this->viewSize = viewSize;
	transform.setOrigin(viewSize.x / 2, viewSize.y / 2);
}

sf::Vector2f CartesianGraphView::getSize() const
{
	return viewSize;
}

void CartesianGraphView::setZoom(const sf::Vector2f& zoom)
{
	if (zoom.x == 0 || zoom.y == 0) throw "Division by 0 error";
	transform.setScale({1 / zoom.x, 1 / zoom.y});
	if (transform.getScale().x == 0.f || transform.getScale().y == 0.f) throw "Division by non-0 resulted in 0";
}

sf::Vector2f CartesianGraphView::getZoom() const
{
	return {1 / transform.getScale().x, 1 / transform.getScale().y};
}

void CartesianGraphView::setViewRect(const sf::FloatRect& rect)
{
	setCenter({rect.left + rect.width / 2, rect.top + rect.height / 2});
	setSize({rect.width, rect.height});
}

sf::FloatRect CartesianGraphView::getViewRect() const
{
	auto viewSize = getSize();
	auto viewPosition = getCenter() - sf::Vector2f(viewSize.x / 2, viewSize.y / 2);
	return {viewPosition.x, viewPosition.y, viewSize.x, viewSize.y};
}

void CartesianGraphView::setCenter(const sf::Vector2f& position)
{
	transform.setPosition(position);
}

sf::Vector2f CartesianGraphView::getCenter() const
{
	return transform.getPosition();
}

void CartesianGraphView::move(const sf::Vector2f& offset)
{
	transform.move(offset);
}

const sf::Transform& CartesianGraphView::getTransform() const
{
	return transform.getTransform();
}

const sf::Transform& CartesianGraphView::getInverseTransform() const
{
	return transform.getInverseTransform();
}
