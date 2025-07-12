#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.h"

class Enemy {
public:
    Enemy(sf::Vector2f position, const sf::Texture& texture, TileMap* map);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    void takeHit();
    bool getIsAlive() const;

    void setScale(float x, float y);
    const sf::Texture& getTexture() const;

    sf::FloatRect getGlobalBounds() const;

    sf::Vector2f getPosition()
    {
        return sprite.getPosition();
    }

private:
    sf::Sprite sprite;
    const sf::Texture& texture;
    TileMap* tileMap;

    bool isAlive = true;
    int hitCount = 0;
    const int maxHits = 3;

    float moveSpeed = 100.f;
    sf::Vector2f direction;
    float switchTime = 1.4f;
    float timeSinceSwitch = 0.f;

    void handleTileCollision();
};
