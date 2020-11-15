#pragma once

#include "CartesianGraphView.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transform.hpp>

class CartesianGrid : public sf::Drawable
{
public:
	CartesianGrid(sf::Vector2f gap = {1, 1}, sf::FloatRect viewRegion = {0, 0, 0, 0});

	sf::Vector2f getGap() const;
	void setGap(const sf::Vector2f& gap);

	void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const;

	void setView(const CartesianGraphView& view);
	const CartesianGraphView& getView() const;

	sf::Color getColor() const;
	void setColor(sf::Color color);

	void update(bool force = false);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Vector2f gap;

	sf::VertexArray mesh = sf::VertexArray(sf::Lines);
	sf::Color color = sf::Color::White;

	sf::Vector2f size;

	CartesianGraphView view;
	sf::Transformable stretchTransform;

	bool needUpdate = true;

	struct Span
	{
		float from = 0;
		float to = 0;
	};

	void createHorizontalLine(float yPosition, const Span& xViewSpan);
	void createVerticalLine(float xPosition, const Span& yViewSpan);

	void createHorizontalLines(const sf::FloatRect& viewRegion);
	void createVerticalLines(const sf::FloatRect& viewRegion);

	void updateGrid();

	void recalculateStretchTransform();
};
