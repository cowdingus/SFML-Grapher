#pragma once

#include "CartesianGrid.hpp"
#include "CartesianGraphView.hpp"

#include <SFML/Graphics.hpp>

class CartesianGraph : public sf::Drawable, protected sf::Transformable
{
public:
	CartesianGraph();

	using sf::Transformable::setPosition;
	using sf::Transformable::getPosition;

	void setView(const CartesianGraphView& view);
	const CartesianGraphView& getView() const;

	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const;

	void setGridGap(const sf::Vector2f& gap);
	sf::Vector2f getGridGap() const;

	void setGridColor(sf::Color color);
	sf::Color getGridColor() const;

	void update();
	virtual void updateGraph() = 0;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	virtual ~CartesianGraph();

protected:
	sf::Vector2f getPointPosition(sf::Vector2f coords) const;

	CartesianGraphView view;
	sf::Transformable stretchTransform; // Prevents the graph from stretching when zooming in/out

	sf::Vector2f size;

	CartesianGrid grid;

	bool needUpdate = true;
	
	struct Span
	{
		float from = 0;
		float to = 0;
	};

private:
	void recalculateStretchTransform();
};
