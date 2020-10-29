#pragma once

#include <SFML/Graphics.hpp>

class CartesianCoordinateSystem : private sf::Transformable
{
private:
	sf::Transformable modifier;

public:
	CartesianCoordinateSystem(
	    const sf::Vector2f& center = {0, 0},
	    const sf::Vector2f& scale = {1, 1},
	    float rotation = 0
	);

	sf::Vector2f mapToWorldCoords(sf::Vector2f localCoords) const;
	sf::Vector2f mapToWorldCoords(float x, float y) const;

	sf::Vector2f mapToLocalCoords(sf::Vector2f worldCoords) const;
	sf::Vector2f mapToLocalCoords(float x, float y) const;

	sf::FloatRect mapToWorldCoords(const sf::FloatRect& rect) const;
	sf::FloatRect mapToLocalCoords(const sf::FloatRect& rect) const;


	void setScale(const sf::Vector2f& factors);
	void setScale(float factorX, float factorY);

	using sf::Transformable::setPosition;
	using sf::Transformable::getPosition;
	using sf::Transformable::setRotation;
	using sf::Transformable::getRotation;
	using sf::Transformable::getTransform;
	using sf::Transformable::getInverseTransform;

	sf::Vector2f getScale() const;


	/*
	 * @brief Get basis vector transform without translation
	 */
	sf::Transform getAxisTransform() const;
	sf::Transform getInverseAxisTransform() const;
};
