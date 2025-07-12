#include "../include/BulletManager.h"

void BulletManager::spawnBullet(sf::Vector2f pos, float dir, bool fasingRight) {
    bullets.emplace_back(pos, dir, fasingRight);//创建一个新的 Bullet 实例，并添加到 bullets 容器
}

void BulletManager::update(float dt) {
    for (auto& b : bullets)
        b.update(dt);// 更新每个子弹的位置和状态

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.isDead(); }), bullets.end());//移除所有已死亡的子弹
}

void BulletManager::draw(sf::RenderWindow& window) {
    for (auto& b : bullets)
        b.draw(window);
}

const std::vector<Bullet>& BulletManager::getBullets() const {
    return bullets;
}
std::vector<Bullet>& BulletManager::getBullets() {
    return bullets;
}
