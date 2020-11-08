#pragma once

#include "CartesianCoordinateSystem.hpp"
#include "CartesianGrid.hpp"

#include <SFML/Graphics.hpp>

class LineGraph final : public sf::Drawable, private sf::Transformable
{
private:
	CartesianCoordinateSystem coordinateSystem;
	CartesianGrid grid;

	std::vector<sf::Vector2f> graphPoints;

	sf::VertexArray mesh = sf::VertexArray(sf::Quads);
	sf::RenderTexture canvas;
	sf::Sprite display;

	sf::FloatRect viewRect;
	sf::Transformable viewTransform;    // What handles the view position, scale, rotation
	sf::Transformable stretchTransform; // Prevents the graph from stretching when zooming in/out

	bool needUpdate = true;

	struct Span
	{
		float from = 0;
		float to = 0;
	};

	const std::array<sf::Vertex, 4> dot
	{
		{
			{{-0.5, 0}, sf::Color::White},
			{{0, 0.5}, sf::Color::White},
			{{0.5, 0}, sf::Color::White},
			{{0, -0.5}, sf::Color::White}
		}
	};

	sf::Vector2f getPointPosition(sf::Vector2f coords) const;
	void createPoint(const sf::Vector2f& coords);

	void recalculateStretchTransform(const sf::Vector2f& canvasViewSize);

	void updateGraph();
	void updateCanvas();

public:
	LineGraph();

	LineGraph(sf::Vector2f size);

	sf::Vector2f getPoint(std::size_t index) const;
	std::size_t getPointsCount() const;
	void addPoint(sf::Vector2f datum);
	void removePoint(std::size_t index);
	void clearPoints();

	void setUnitScaling(const sf::Vector2f& unitScaling);
	sf::Vector2f getUnitScaling() const;

	void setSize(sf::Vector2f size);
	sf::Vector2u getSize() const;

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

	using sf::Transformable::setPosition;

	void update();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
