#pragma once

#include "CartesianCoordinateSystem.hpp"

#include <SFML/Graphics.hpp>

class CartesianGrid : public sf::Drawable
{
private:
	CartesianCoordinateSystem& ccs;
	sf::FloatRect viewRect;

	sf::Vector2f gap;

	sf::VertexArray mesh = sf::VertexArray(sf::Lines);
	sf::Color color = sf::Color::White;

	sf::Transform viewTransform;
	sf::Transform stretchTransform;

	struct Span
	{
		float from = 0;
		float to = 0;
	};

	void createHorizontalLine(float yPosition);
	void createVerticalLine(float xPosition);

	void createHorizontalLines(const sf::FloatRect& viewRegion);
	void createVerticalLines(const sf::FloatRect& viewRegion);

	void updateGrid();

public:
	CartesianGrid(CartesianCoordinateSystem& ccs, sf::Vector2f gap = {1, 1}, sf::FloatRect viewRegion = {0, 0, 0, 0});

	sf::Vector2f getGap() const;
	void setGap(const sf::Vector2f& gap);

	const sf::FloatRect& getViewRegion() const;
	void setViewRegion(const sf::FloatRect& viewRegion);
	void setViewRegion(const sf::Vector2f& topLeftPosition, const sf::Vector2f& size);
	void moveViewRegion(const sf::Vector2f& offset);

	sf::Color getColor() const;
	void setColor(sf::Color color);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setViewTransform(const sf::Transform& transform);
	void setStretchTransform(const sf::Transform& transform);
};
