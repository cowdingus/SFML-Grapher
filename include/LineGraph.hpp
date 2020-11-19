#pragma once

#include "CartesianGraph.hpp"

#include <SFML/Graphics.hpp>

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

private:
	std::vector<sf::Vector2f> points;

	sf::VertexArray mesh = sf::VertexArray(sf::Lines);

	bool needUpdate = true;

	void createLine(const sf::Vector2f& from, const sf::Vector2f& to);

	void render(sf::RenderTexture& texture) const override;
	void updateContent() override;
};
