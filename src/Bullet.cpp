#include "../include/Bullet.h"

Bullet::Bullet(sf::Vector2f pos, float dir, bool facingRight) {
    shape.setRadius(4.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(pos);
    velocity.x = dir * 200.f; // 快速移动
    if (facingRight == 0)  velocity = -velocity;
}

void Bullet::update(float dt)   {
    sf::Vector2f movement = velocity * dt;
    distanceTraveled += std::abs(movement.x);
    shape.move(movement);
    if (distanceTraveled >= maxDistance)
        dead = true;
}

void Bullet::setAlive(bool alive) {
    dead = !alive;
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Bullet::isDead() const {
    return dead;
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}
sf::FloatRect Bullet::getGlobalBounds() const {
    return shape.getGlobalBounds(); // 为统一接口命名
}