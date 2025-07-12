#pragma once
#include "Bullet.h"

class BulletManager {
private:
    std::vector<Bullet> bullets;

public:
    void spawnBullet(sf::Vector2f pos, float dir,bool fasingRight);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets();
};
