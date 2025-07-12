#include "../include/Enemy.h"

Enemy::Enemy(sf::Vector2f position, const sf::Texture& texture, TileMap* map)
    : texture(texture), tileMap(map), sprite(texture) {
    sprite.setTexture(this->texture);
    sprite.setPosition(position);
    direction = sf::Vector2f(1.f, 0.f); // 默认向右
}

void Enemy::update(float deltaTime) {
    if (!isAlive) return;

    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // ====== 重力下落逻辑 ======
    float gravity = 300.f;
    float fallSpeed = gravity * deltaTime;

    int colLeft = static_cast<int>(pos.x) / tileMap->getTileWidth();
    int colRight = static_cast<int>(pos.x + bounds.size.x - 1) / tileMap->getTileWidth();
    int rowBelow = static_cast<int>(pos.y + bounds.size.y + fallSpeed) / tileMap->getTileHeight();

    bool isStandingOnGround =
        tileMap->getTileType(rowBelow, colLeft) == TileType::ground ||
        tileMap->getTileType(rowBelow, colRight) == TileType::ground;

    if (!isStandingOnGround) {
        // 向下掉落
        sprite.move({ 0.f, fallSpeed });
        return; // 在空中不进行水平移动
    }

    // ====== 水平移动逻辑 ======
    timeSinceSwitch += deltaTime;
    if (timeSinceSwitch >= switchTime) {
        direction.x = -direction.x;
        timeSinceSwitch = 0.f;
    }

    handleTileCollision(); // 判断前方是否撞墙

    float dx = direction.x * moveSpeed * deltaTime;
    sprite.move({ dx, 0.f });
}


void Enemy::handleTileCollision() {
    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    int rowTop = static_cast<int>(pos.y) / tileMap->getTileHeight();
    int rowBottom = static_cast<int>(pos.y + bounds.size.y - 1) / tileMap->getTileHeight();

    // 判断“前方”的列
    int colFront = direction.x > 0 ?
        static_cast<int>(pos.x + bounds.size.x) / tileMap->getTileWidth() :
        static_cast<int>(pos.x - 1) / tileMap->getTileWidth();

    // 检测上下两行前方是否有障碍
    for (int row = rowTop; row <= rowBottom; ++row) {
        if (tileMap->getTileType(row, colFront) == TileType::ground) {
            direction.x = -direction.x; // 撞到障碍，反转
            timeSinceSwitch = 0.f; // 重置反转时间
            break;
        }
    }
}


void Enemy::draw(sf::RenderWindow& window) {
    if (isAlive)
        window.draw(sprite);
}

void Enemy::takeHit() {
    hitCount++;
    if (hitCount >= maxHits) {
        isAlive = false;
    }
}

bool Enemy::getIsAlive() const {
    return isAlive;
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}


void Enemy::setScale(float x, float y) {
    sprite.setScale({ x, y });
}

const sf::Texture& Enemy::getTexture() const {
    return texture;
}
