#pragma once

#include "CartesianGrid.hpp"

#include <SFML/Graphics.hpp>

class CartesianGraph : protected sf::Transformable
{
public:
	CartesianGraph();

	using sf::Transformable::setPosition;
	using sf::Transformable::getPosition;

	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const;

	void setZoom(sf::Vector2f zoom);
	sf::Vector2f getZoom() const;

	void setViewRect(const sf::FloatRect& rect);
	sf::FloatRect getViewRect() const;

	void setViewPosition(const sf::Vector2f& position);
	sf::Vector2f getViewPosition() const;

	void setViewSize(const sf::Vector2f& size);
	sf::Vector2f getViewSize() const;

	void moveViewPosition(const sf::Vector2f& offset);

	void setGridGap(const sf::Vector2f& gap);
	sf::Vector2f getGridGap() const;

	void setGridColor(sf::Color color);
	sf::Color getGridColor() const;

	void update();
	virtual void updateGraph() = 0;

	virtual ~CartesianGraph();

protected:
	sf::Vector2f getPointPosition(sf::Vector2f coords) const;

	CartesianGrid grid;

	sf::FloatRect viewRect;
	sf::Transformable viewTransform;    // What handles the view position, scale, rotation
	sf::Transformable stretchTransform; // Prevents the graph from stretching when zooming in/out

	sf::Vector2f size;

	bool needUpdate = true;

private:
	void recalculateStretchTransform(const sf::Vector2f& canvasViewSize);
};
