#pragma once

#include "CartesianGraph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

class LineGraph : public CartesianGraph
{
public:
	LineGraph();

	LineGraph(const sf::Vector2f& size);

	std::size_t getPointsCount() const;
	sf::Vector2f getPoint(std::size_t index) const;
	void addPoint(const sf::Vector2f& point);
	void removePoint(std::size_t index);
	void clearPoints();

	void setLineThickness(float thickness);
	float getLineThickness() const;

	void setLineColor(sf::Color color);
	sf::Color getLineColor() const;

private:
	sf::Color lineColor = sf::Color::White;
	float lineThickness = 1.f;

	std::vector<sf::Vector2f> points;

	sf::VertexArray mesh = sf::VertexArray(sf::TriangleStrip);

	std::array<sf::Vector2f, 2> createLineSegment(const sf::Vector2f& point);	

	void render(sf::RenderTexture& texture) const override;
	void updateContent() override;
};
