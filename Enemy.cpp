#include "Enemy.hpp"
#include "Kirby.hpp"
#include <iostream> // 用于调试输出
#include <cmath>    // 用于 std::sqrt, std::pow


// 构造函数
Enemy::Enemy(sf::Vector2f spawnPos, sf::Vector2f pA, sf::Vector2f pB, const sf::Texture& texture, int amount) :
    Entity(spawnPos.x, spawnPos.y, texture), // 调用基类 Entity 的构造函数
    attackDamage(amount),
    patrolPointA(pA),
    patrolPointB(pB)
{
    // 如果 敌人 的图形形状 (shape) 用于定义其固定大小或碰撞体：
    shape.setSize(sf::Vector2f(50.f, 50.f)); // 例如，设置 敌人 的固定大小为 50x50 像素
    shape.setFillColor(sf::Color::White);  // 默认颜色

    // 确保巡逻点A和B是不同的，否则可能会原地不动
    if (patrolPointA == patrolPointB) {
        // 如果相同，默认让它在 A 点的右边一点巡逻
        patrolPointB.x += 100.f;
    }

    // 初始化当前目标点为 A 或 B，取决于哪个更近或你希望的起始方向
    // 这里我们默认先向 B 点移动
    currentTargetPoint = patrolPointB;
    movingToB = true;


    // 设置精灵的初始纹理矩形（假设纹理文件只包含一个敌人图像，或者第一个是默认帧）
    // 如果你有动画精灵表，这里会是第一帧的矩形
    currentTextureRect = sf::IntRect({ 0, 0 }, { 30, 30 }); // 假设敌人纹理尺寸是 30x30
    sprite.setTextureRect(currentTextureRect);

    // 设置精灵位置
    sprite.setPosition(position);
    // 可选：设置精灵原点，如果需要进行翻转
    // sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}


// 设置 Kirby 实例的指针
void Enemy::setKirbyTarget(Kirby* kirby) {
    kirbyPtr = kirby;
}

// 更新敌人状态和行为
void Enemy::update(float deltaTime) {
    if (!isAlive) {
        // 如果敌人已死亡，停止一切行为，可选：播放死亡动画，最终从场景移除
        velocity = sf::Vector2f(0.f, 0.f);
        return;
    }

    patrol(deltaTime); // 执行巡逻行为

    // 应用速度到位置
    position += velocity * deltaTime;
    sprite.setPosition(position);
    boundingBox = sprite.getGlobalBounds(); // 更新碰撞边界框

    // 检查并攻击 Kirby
    checkAndAttackKirby();
}

// 绘制敌人
void Enemy::draw(sf::RenderWindow& window) {
    if (isAlive) { // 只有活着的敌人才能被绘制
        window.draw(sprite);
    }
}

//设置纹理矩形
void Enemy::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    // 每次纹理矩形改变后，如果Kirby的视觉尺寸需要保持不变，需要重新调整缩放或原点
    // 这里我们假设Kirby的大小由shape决定，sprite需要适应它
    if (sprite.getLocalBounds().size.x > 0 && sprite.getLocalBounds().size.y > 0) {
        sprite.setScale({ shape.getSize().x / sprite.getLocalBounds().size.x,
            shape.getSize().y / sprite.getLocalBounds().size.y });
    }
    // 更新碰撞框以匹配新的精灵尺寸（如果纹理矩形影响了尺寸）
    boundingBox = sprite.getGlobalBounds();
}

// 受到伤害
void Enemy::takeDamage(int amount) {
    if (!isAlive) { // 死了就不能再受伤了
        return;
    }

    health -= amount;
    std::cout << "敌人受到 " << amount << " 点伤害！当前生命值: " << health << std::endl;

    if (health <= 0) {
        health = 0;
        isAlive = false; // 标记敌人为死亡
        std::cout << "敌人已被击败！" << std::endl;
        // 播放死亡动画，或触发其他死亡效果（例如消失）
        // sprite.setColor(sf::Color(255, 255, 255, 128)); // 变半透明表示死亡
    }
    else {
        // 可选：播放受击动画或音效
        sprite.setColor(sf::Color::Red); // 短暂变红表示受击
        // 重置颜色
        // static sf::Clock hitFlashTimer;
        // hitFlashTimer.restart();
        // 可以在 update 中检测这个计时器来恢复颜色
    }
}

// 巡逻行为
void Enemy::patrol(float deltaTime) {
    sf::Vector2f target = currentTargetPoint;
    sf::Vector2f direction = target - position; // 计算方向向量

    // 计算距离目标点的距离
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f) { // 如果距离目标点非常近 (阈值 5.0f 避免浮点数误差)
        // 到达当前目标点，切换目标
        if (movingToB) {
            currentTargetPoint = patrolPointA; // 切换到 A 点
            movingToB = false;
        }
        else {
            currentTargetPoint = patrolPointB; // 切换到 B 点
            movingToB = true;
        }
        // 确保速度归零，等待下一帧重新计算方向
        velocity.x = 0;
        // 可选：在这里改变敌人朝向
        // sprite.setScale(movingToB ? 1.f : -1.f, 1.f);
    }
    else {
        // 向目标点移动
        // 归一化方向向量并乘以速度
        velocity.x = (direction.x / distance) * moveSpeed;
        // 如果有垂直方向的巡逻，也要设置 velocity.y

        // 根据移动方向更新精灵朝向 (如果精灵原点在中心，-1.f 会翻转)
        if (velocity.x > 0) {
            sprite.setScale({ 1.f, 1.f }); // 朝右
        }
        else if (velocity.x < 0) {
            sprite.setScale({ -1.f, 1.f }); // 朝左
        }
    }
}

// 检查并攻击 Kirby
void Enemy::checkAndAttackKirby() {
    if (!isAlive || kirbyPtr == nullptr || !kirbyPtr->getIsAlive()) {
        return; // 敌人或 Kirby 已死亡或 Kirby 实例未设置
    }

    // 如果敌人和 Kirby 的碰撞边界重叠
    if (boundingBox.findIntersection(kirbyPtr->getGlobalBounds())) {
        // 检查攻击冷却时间
        if (attackCooldownTimer.getElapsedTime().asSeconds() >= attackCooldown) {
            std::cout << "敌人接触到 Kirby，造成伤害！" << std::endl;
            kirbyPtr->takeDamage(attackDamage); // Kirby 受到伤害 (Kirby 类的 takeDamage() 不需要参数)
            attackCooldownTimer.restart(); // 重置攻击冷却计时器
        }
    }
}
