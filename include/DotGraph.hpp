#pragma once

#include "CartesianGrid.hpp"
#include "CartesianGraph.hpp"

#include <SFML/Graphics.hpp>

#include <array>

class DotGraph final : public sf::Drawable, public CartesianGraph
{
public:
	DotGraph();

	DotGraph(sf::Vector2f size);

	void setSize(const sf::Vector2f& size);

	sf::Vector2f getPoint(std::size_t index) const;
	std::size_t getPointsCount() const;
	void addPoint(sf::Vector2f datum);
	void removePoint(std::size_t index);
	void clearPoints();

	void updateGraph() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<sf::Vector2f> graphPoints;

	sf::VertexArray mesh = sf::VertexArray(sf::Quads);

	bool needUpdate = true;

	sf::RenderTexture canvas;
	sf::Sprite display;

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

	void createPoint(const sf::Vector2f& coords);

	void updateContent();
	void updateCanvas();
};
