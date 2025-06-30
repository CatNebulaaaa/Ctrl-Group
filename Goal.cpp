#include "Goal.hpp"

// 构造函数实现
Goal::Goal(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect)
    : Entity(x, y, texture) // 调用基类构造函数
{
    // 设置精灵的纹理矩形
    setSpriteTextureRect(textureRect);

    // 设置碰撞边界
    boundingBox = sprite.getGlobalBounds(); // 目标通常也需要一个碰撞边界来检测主角是否到达
    this->isAlive = true; // 目标默认是“活着”的
    this->velocity = sf::Vector2f(0.f, 0.f); // 目标通常不移动
    std::cout << "Goal created at (" << position.x << ", " << position.y << "), size ("
        << boundingBox.size.x << ", " << boundingBox.size.y << ")" << std::endl;
}

// 设置精灵纹理矩形的实现
void Goal::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f });
    sprite.setPosition(position); // 确保精灵位置与实体位置同步
}

// update 函数实现
// 目标通常不需要更新，除非它有动画或特殊效果
void Goal::update(float deltaTime) {
    // 通常为空
    // 如果有动画，animation->update(deltaTime);
}

// draw 函数实现
void Goal::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
