#include "../include/Entity.hpp"

sf::FloatRect Entity::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

Entity::Entity(float x, float y, const sf::Texture& texture) :position(x, y), velocity(0.f, 0.f), isAlive(true), sprite(texture)
{
	boundingBox = sf::FloatRect({ position.x, position.y }, { 0.f, 0.f });
}