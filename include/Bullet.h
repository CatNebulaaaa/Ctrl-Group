#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float distanceTraveled = 0.f;
    float maxDistance = 200.f;
    bool dead = false;

    Bullet(sf::Vector2f pos, float dir, bool facingRight);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isDead() const;
    void setAlive(bool alive);

    sf::FloatRect getBounds() const; 
    sf::FloatRect getGlobalBounds() const;

    bool intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
        return a.position.x < b.position.x + b.position.x &&
            a.position.x + a.size.x > b.position.x &&
            a.position.y < b.position.y + b.size.y &&
            a.position.y + a.size.y > b.position.y;
    }
};

