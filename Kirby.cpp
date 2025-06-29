#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // ���ڵ������
#include <optional> // ���� SFML 3.0.0 ���¼�����
#include "Kirby.hpp"

Kirby::Kirby(const sf::Texture& texture, sf::Vector2f spawnPos)
{
	sprite.setTexture(texture);
	sprite.setPosition(spawnPos);
	position = spawnPos;
	isAlive = true;

	inhaleRange = sf::FloatRect(position.x + 20.f, position.y, 50.f, 50.f);
}

