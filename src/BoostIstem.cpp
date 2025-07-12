#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // 用于调试输出
#include <optional> // 用于 SFML 3.0.0 的事件处理
#include <algorithm>
#include <cmath>

#include "../include/BoostItem.hpp"
#include "../include/Kirby.h"

class Kirby;

BoostItem::BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp) :
    Entity(spawnPos.x, spawnPos.y, texture),
    type(tp),
    baseY(spawnPos.y)  // 添加 baseY 存储初始 Y 位置
{
    // 获取纹理的完整尺寸
    sf::Vector2u textureSize = texture.getSize();

    // 设置精灵使用整个纹理
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(textureSize.x), static_cast<int>(textureSize.y) }));

    // 计算合适的缩放比例
    float desiredWidth = 10.0f;
    scaleFactor = desiredWidth / static_cast<float>(textureSize.x);  // 使用成员变量

    // 应用缩放
    sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));

    // 设置初始位置
    sprite.setPosition(position);

    // 更新碰撞框
    boundingBox = sprite.getGlobalBounds();

    // 调试输出
    std::cout << "BoostItem created. Scaled size: "
        << boundingBox.size.x << "x" << boundingBox.size.y << "\n";
}

//如果没被吃掉就绘制果实
void BoostItem::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(sprite);
    }
}

//添加一个上下浮动的动作
void BoostItem::update(float deltaTime) {
    float elapsedTime = floatClock.getElapsedTime().asSeconds();
    float yOffset = std::sin(elapsedTime * floatSpeed) * floatAmplitude;

    // 使用 baseY 作为基础位置
    float currentY = baseY + yOffset;
    sprite.setPosition(sf::Vector2f(position.x, currentY));

    // 更新碰撞框
    boundingBox = sprite.getGlobalBounds();

    // 同步位置（可选，如果需要基类位置更新）
    position.y = currentY;
}

void BoostItem::handleCollision(Kirby& kirby) {
    if (kirby.isDead()) return;

    Kirby* kirbyPtr = &kirby;
    if (kirbyPtr && !kirbyPtr->isDead()) {
        sf::FloatRect itemBounds = getGlobalBounds();
        sf::FloatRect kirbyBounds = kirbyPtr->getGlobalBounds();

        // 使用实际尺寸计算面积
        float itemArea = itemBounds.size.x * itemBounds.size.y;

        if (std::optional<sf::FloatRect> intersection = itemBounds.findIntersection(kirbyBounds)) {
            float intersectionArea = intersection->size.x * intersection->size.y;
            boost(&kirby);
            // 使用实际面积计算阈值
            if (intersectionArea >= itemArea * 0.1f) { // 10% 重叠
                boost(kirbyPtr);
                setAlive(false);
            }
        }
    }
}

//设置纹理矩形
void BoostItem::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    // 每次纹理矩形改变后，如果Kirby的视觉尺寸需要保持不变，需要重新调整缩放或原点
    // 这里我们假设Kirby的大小由shape决定，sprite需要适应它
    if (sprite.getLocalBounds().size.x > 0 && sprite.getLocalBounds().size.y > 0) {
        sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));
    }
    // 更新碰撞框以匹配新的精灵尺寸（如果纹理矩形影响了尺寸）
    boundingBox = sprite.getGlobalBounds();
}


void BoostItem::boost(Kirby* kirby)
{
    if (!isAlive) {
        return;
    }

    std::cout << "Kirby 捡到了增益果实: ";

    switch (type) { // 根据 BoostItem 的类型来判断
    case BoostType::HEAL:
        // 治疗效果：增加 Kirby 的当前生命值，但不超过最大生命值
        kirby->health = std::min(kirby->health + 25, kirby->maxHealth);
        std::cout << "治疗！生命值小于100则对当前生命值加25 " << kirby->health << std::endl;
        break;

    case BoostType::INVINCIBLE:
        // 无敌效果：设置 Kirby 为无敌状态，并启动计时器
        kirby->isInvincible = true;
        kirby->invincibilityTimer.restart(); // 重启无敌计时器
        std::cout << "获得无敌效果！" << std::endl;
        break;
    }
    // 增益果实被“吃掉”后，将其标记为不活跃，以便从游戏世界中移除
    setAlive(false);
}


