#include "Obstacle.hpp"

// 构造函数实现
Obstacle::Obstacle(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect, bool solid)
    : Entity(x, y, texture) // 调用基类构造函数
{
    // 设置精灵的纹理矩形
    setSpriteTextureRect(textureRect);

    boundingBox = sprite.getGlobalBounds();
    this->isAlive = true; // 障碍物通常默认是“活着”的
    this->velocity = sf::Vector2f(0.f, 0.f); // 障碍物通常不移动
    std::cout << "Obstacle created at (" << position.x << ", " << position.y << "), size ("
        << boundingBox.size.x << ", " << boundingBox.size.y << ")" << std::endl;
}

// 设置精灵纹理矩形的实现
void Obstacle::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f});
    sprite.setPosition(position); // 确保精灵位置与实体位置同步
}

// update 函数实现
// 障碍物通常不需要更新，除非它有动画或特殊行为
void Obstacle::update(float deltaTime) {
    // 障碍物通常是静态的，所以这里可能为空
    // 如果有动画，animation->update(deltaTime);
}

// draw 函数实现
void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}