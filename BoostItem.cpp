#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // 用于调试输出
#include <optional> // 用于 SFML 3.0.0 的事件处理
#include <algorithm>
#include <cmath>

#include "BoostItem.hpp"
#include "Kirby.hpp"

class Kirby;

BoostItem::BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp) :
	Entity(spawnPos.x, spawnPos.y, texture),
	type(type){ }

//如果没被吃掉就绘制果实
void BoostItem::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(sprite);
    }
}

//添加一个上下浮动的动作
void BoostItem::update(float deltaTime) {
    // 获取自计时器重启以来的总时间（秒）
    float elapsedTime = floatClock.getElapsedTime().asSeconds();

    // 计算 Y 轴的浮动偏移量
    // std::sin 函数接受弧度，floatSpeed 可以控制浮动频率
    float yOffset = std::sin(elapsedTime * floatSpeed) * floatAmplitude;

    // 将浮动偏移量应用到精灵的 Y 坐标上
    // 注意：`position.x` 保持不变，`position.y` 加上基类 `position` 的 Y 坐标和偏移量
    sprite.setPosition({ position.x, position.y + yOffset });

    // 更新碰撞框的位置，以匹配浮动后的精灵位置
    boundingBox = sprite.getGlobalBounds();
}

void BoostItem::handleCollision(Entity& other) {
    // 1. 确保 BoostItem 处于活动状态
    if (!isAlive) {
        return;
    }

    // 2. 尝试将 other 转换为 Kirby 类型
    // static_cast 也可以，但 dynamic_cast 更安全，如果 other 不是 Kirby，它会返回 nullptr
    Kirby* kirbyPtr = dynamic_cast<Kirby*>(&other);

    // 3. 如果 other 确实是 Kirby 并且 Kirby 也活着
    if (kirbyPtr && kirbyPtr->getIsAlive()) {
        // 获取 BoostItem 和 Kirby 的碰撞边界框
        sf::FloatRect boostItemBounds = getGlobalBounds();
        sf::FloatRect kirbyBounds = kirbyPtr->getGlobalBounds();

        // 检查是否有任何重叠
        if (const std::optional intersection = boundingBox.findIntersection(kirbyPtr->getGlobalBounds())) {
            float intersectionArea = intersection->size.x * intersection->size.y;
            // 判断重叠面积是否超过 BoostItem 自身面积的一半
            if (intersectionArea >= (size * 0.5f)) {
                // 如果满足条件，激活增益效果
                boost(kirbyPtr); // 调用友元函数，传入 Kirby 引用
            }
        }
    }
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
        std::cout << "治疗！当前生命值: " << kirby->health << std::endl;
        break;

    case BoostType::DAMAGEUP:
        // 增伤效果：提升 Kirby 的攻击力，并启动计时器
        kirby->currentAttackDamage = kirby->attackDamage * 2.0f; // 攻击力翻倍
        kirby->hasAttackBoost = true;
        kirby->attackBoostTimer.restart(); // 重启攻击力提升计时器
        std::cout << "攻击力提升！" << std::endl;
        break;

    case BoostType::INVINCIBLE:
        // 无敌效果：设置 Kirby 为无敌状态，并启动计时器
        kirby->isInvincible = true;
        kirby->invincibilityTimer.restart(); // 重启无敌计时器
        std::cout << "获得无敌效果！" << std::endl;
        break;

    case BoostType::HASTE:
        //加速效果
        kirby->velocity.x *= 1.5f; // 速度提升 50%
        kirby->hasSpeedBoost = true;
        kirby->speedBoostTimer.restart();
        std::cout << "获得加速效果！" << std::endl;
        break;
    }
    // 增益果实被“吃掉”后，将其标记为不活跃，以便从游戏世界中移除
    isAlive = false;
}


