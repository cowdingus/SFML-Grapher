#include <CartesianCoordinateSystem.hpp>

#include <SFML/Graphics.hpp>

CartesianCoordinateSystem::CartesianCoordinateSystem(const sf::Vector2f& center, const sf::Vector2f& scale, float rotation)
{
	setPosition(center);
	setScale(scale);
	setRotation(0);
}

sf::Vector2f CartesianCoordinateSystem::mapToWorldCoords(sf::Vector2f localCoords) const
{
	return getTransform().transformPoint(localCoords);
}

sf::Vector2f CartesianCoordinateSystem::mapToWorldCoords(float x, float y) const
{
	return mapToWorldCoords({x, y});
}

sf::Vector2f CartesianCoordinateSystem::mapToLocalCoords(sf::Vector2f worldCoords) const
{
	return getTransform().getInverse().transformPoint(worldCoords);
}

sf::Vector2f CartesianCoordinateSystem::mapToLocalCoords(float x, float y) const
{
	return mapToLocalCoords({x, y});
}

sf::FloatRect CartesianCoordinateSystem::mapToWorldCoords(const sf::FloatRect& rect) const
{
	const sf::Transform& at = getAxisTransform();

	sf::Vector2f position = mapToWorldCoords({rect.left, rect.top});
	sf::Vector2f size = at.transformPoint({rect.width, rect.height});
	sf::FloatRect worldRect{position.x, position.y, size.x, size.y};

	return worldRect;
}

sf::FloatRect CartesianCoordinateSystem::mapToLocalCoords(const sf::FloatRect& rect) const
{
	const sf::Transform& iat = getInverseAxisTransform();

	sf::Vector2f position = mapToLocalCoords({rect.left, rect.top});
	sf::Vector2f size = iat.transformPoint({rect.width, rect.height});
	sf::FloatRect localRect{position.x, position.y, size.x, size.y};

	return localRect;
}

void CartesianCoordinateSystem::setScale(const sf::Vector2f& factors)
{
	sf::Transformable::setScale(factors.x, -factors.y);
}

void CartesianCoordinateSystem::setScale(float factorX, float factorY)
{
	sf::Transformable::setScale(factorX, -factorY);
}

sf::Vector2f CartesianCoordinateSystem::getScale() const
{
	sf::Vector2f scale = sf::Transformable::getScale();
	scale.y = -scale.y;
	return scale;
}

/*
 * @brief Get basis vector transform without translation
 */
sf::Transform CartesianCoordinateSystem::getAxisTransform() const
{
	sf::Transform t;
	t.translate(-getPosition());
	t.combine(getTransform());

	return t;
}
sf::Transform CartesianCoordinateSystem::getInverseAxisTransform() const
{
	sf::Transform t = getAxisTransform().getInverse();
	return t;
}
