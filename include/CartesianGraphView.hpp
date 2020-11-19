#pragma once

#include <SFML/Graphics/Transformable.hpp>

class CartesianGraphView
{
public:
	CartesianGraphView();

	void setZoom(const sf::Vector2f& zoom);
	sf::Vector2f getZoom() const;

	void setViewRect(const sf::FloatRect& rect);
	sf::FloatRect getViewRect() const;

	void setCenter(const sf::Vector2f& position);
	sf::Vector2f getCenter() const;

	void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const;

	void move(const sf::Vector2f& offset);

	const sf::Transform& getTransform() const;
	const sf::Transform& getInverseTransform() const;

private:
	sf::Vector2f viewSize;
	sf::Vector2f position;
	sf::Vector2f scaling;
	sf::Vector2f origin;

	sf::Transformable transform;
};
