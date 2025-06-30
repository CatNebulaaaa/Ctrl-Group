#include <optional>
#include <vector>
#include <memory>

#include "Kirby.hpp"

// ———— 构造函数 ————
Kirby::Kirby(const sf::Texture& texture, sf::Vector2f spawnPos)
    : Entity(spawnPos.x, spawnPos.y,texture), // 调用基类 Entity 的构造函数，初始化 position, velocity, isAlive
    invincibilityDuration(2.0f),   // 设置无敌持续时间
    standingRect({ 0, 0 }, { 50, 100 }),
    crouchingRect({ 50, 0 }, { 50, 50 }),
    attackRect({ 100, 0 }, { 80, 100 }),
    attackDamage(10.f),
    currentAttackDamage(attackDamage)
{
    // 如果 Kirby 的图形形状 (shape) 用于定义其固定大小或碰撞体：
    shape.setSize(sf::Vector2f(50.f, 50.f)); // 例如，设置 Kirby 的固定大小为 50x50 像素
    shape.setFillColor(sf::Color::White);  // 默认颜色

    // 设置精灵的初始纹理矩形和位置
    sprite.setTextureRect(standingRect); // 初始显示站立动画帧
    sprite.setPosition(position);        // 将精灵位置设置为实体（基类）的位置

    // 初始化攻击范围形状（仅用于调试时可视化）
    attackRangeShape.setRadius(attackRadius);
    // 设置攻击范围的原点到其中心，以便于定位
    attackRangeShape.setOrigin({ attackRadius, attackRadius });
    attackRangeShape.setFillColor(sf::Color(255, 0, 0, 50)); // 半透明红色
    attackRangeShape.setOutlineColor(sf::Color::Red);       // 红色边框
    attackRangeShape.setOutlineThickness(1.f);               // 边框粗细
}


//———— 重写基类虚函数 ————


//随时间更新
// update：每帧更新卡比的状态和逻辑
void Kirby::update(float deltaTime) {
    // 处理无敌状态计时和视觉效果
    if (isInvincible) {
        if (invincibilityTimer.getElapsedTime().asSeconds() >= invincibilityDuration) {
            isInvincible = false;
            std::cout << "Kirby 不再无敌。" << std::endl;
            sprite.setColor(sf::Color::White); // 恢复正常颜色
        }
        else {
            // 无敌时使卡比闪烁效果
            if (static_cast<int>(invincibilityTimer.getElapsedTime().asMilliseconds() / 100) % 2 == 0) {
                sprite.setColor(sf::Color(255, 255, 255, 128)); // 半透明
            }
            else {
                sprite.setColor(sf::Color::White); // 不透明
            }
        }
    }

    // 加速效果计时器管理
    if (hasSpeedBoost) {
        if (speedBoostTimer.getElapsedTime().asSeconds() >= speedBoostDuration) {
            hasSpeedBoost = false;
            velocity.x /= 1.5f; // 速度恢复到基础值
            std::cout << "加速效果结束！" << std::endl;
        }
    }

    // 攻击力提升效果计时器管理
    if (hasAttackBoost) {
        if (attackBoostTimer.getElapsedTime().asSeconds() >= attackBoostDuration) {
            hasAttackBoost = false;
            currentAttackDamage = attackDamage; // 攻击力恢复到基础值
            std::cout << "攻击力提升效果结束！" << std::endl;
        }
    }

    // 处理输入（实时按键检测）
    updateInput();
    // 更新游戏逻辑（根据输入和当前状态）
    updateLogic();

    // 应用速度到位置，实现移动
    position += velocity * deltaTime;

    // 将精灵的位置同步到卡比的逻辑位置
    sprite.setPosition(position);

    // 更新碰撞边界框，使其与精灵的当前状态匹配
    boundingBox = sprite.getGlobalBounds();
}

//绘制sprite
void Kirby::draw(sf::RenderWindow& window)
{
    // 调用基类的 draw 方法，它会绘制 sprite
    // 或者直接 window.draw(sprite);
    window.draw(sprite);
}

//设置纹理矩形
void Kirby::setSpriteTextureRect(const sf::IntRect& rect) {
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




//———— Kirby独有的操作 ————

//检测键盘/鼠标的单次输入
void Kirby::handleInput(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // 如果按下E键，执行攻击
        if (keyPressed->code == sf::Keyboard::Key::E)
        {
            attack(enemiesPtr);
        }
    }
}


// updateInput：处理实时输入（每帧检测按键状态）
void Kirby::updateInput() {
    // 左右移动输入 (A/D 键或左右方向键)
    velocity.x = 0.f; // 每帧重置水平速度
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -moveSpeed; // 向左移动
        facingRight = false;    // 朝向左
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = moveSpeed;  // 向右移动
        facingRight = true;     // 朝向右
    }

    // 跳跃输入 (空格键、W 键或上方向键)
    // 只有在地面上且跳跃键被按下时才能跳跃
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) && isGrounded) {
        if (!jumpPressed) { // 避免重复跳跃动作，只在按下瞬间触发
            jumpPressed = true;
            velocity.y = -jumpSpeed; // 施加向上的速度
            isGrounded = false;      // 不再处于地面
        }
    }
    else {
        // 如果跳跃键松开，重置 jumpPressed 标志
        jumpPressed = false;
    }

    // 下蹲输入 (S 键或下方向键)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        crouchPressed = true;
    }
    else {
        crouchPressed = false;
    }

    // 攻击输入 (J 键) - 只有在当前不在攻击状态时才能启动新攻击
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) && !isAttacking) {
        attack(enemiesPtr); // 触发攻击功能
    }
}

void Kirby::updateLogic() {
    // --- 攻击逻辑与动画 ---
    if (isAttacking) {
        // 攻击动画正在进行
        if (attackTimer.getElapsedTime().asSeconds() >= attackDuration) {
            isAttacking = false; // 攻击动画结束
            std::cout << "Kirby 攻击结束。" << std::endl;
            // 攻击结束后恢复默认动画帧
            sprite.setTextureRect(standingRect);
        }
        else {
            // 在攻击持续时间内，显示攻击动画帧
            sprite.setTextureRect(attackRect);
            // 如果攻击动画帧的尺寸与站立帧不同，可能需要调整精灵的位置
            // 例如：如果攻击动画更宽，需要向左偏移以保持中心不变
            // sprite.setPosition(position.x - (attackRect.width - standingRect.width) / 2.f, position.y);
            // 假设攻击时，精灵朝向不变，或者在攻击开始时就设置好朝向
            // 这里可以根据 faciongRight 调整攻击范围的位置，使其在卡比前方
            if (facingRight) {
                attackRangeShape.setPosition({ sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f + attackRadius / 2.f,
                    sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f });
            }
            else {
                attackRangeShape.setPosition({ sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f - attackRadius / 2.f,
                    sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f });
            }
        }

        // --- 攻击期间敌人伤害检测 ---
        // 确保敌人列表存在且非空
        if (enemiesPtr) {
            // 遍历敌人列表，使用迭代器以安全地移除死亡的敌人
            for (auto it = enemiesPtr->begin(); it != enemiesPtr->end(); ) {
                if ((*it) && (*it)->getIsAlive()) { // 确保敌人对象有效且存活
                    sf::FloatRect enemyBounds = (*it)->getGlobalBounds();
                    sf::Vector2f enemyCenter(enemyBounds.position.x + enemyBounds.size.x / 2.f,
                        enemyBounds.position.y + enemyBounds.size.y / 2.f);

                    // 获取卡比攻击范围的中心点
                    sf::Vector2f kirbyAttackCenter = attackRangeShape.getPosition(); // 攻击范围形状的原点已设置为中心

                    // 计算敌人中心到卡比攻击范围中心的距离
                    float distance = std::sqrt(
                        std::pow(enemyCenter.x - kirbyAttackCenter.x, 2) +
                        std::pow(enemyCenter.y - kirbyAttackCenter.y, 2)
                    );

                    // 如果敌人在攻击范围内
                    if (distance < attackRadius) {
                        // 敌人受到伤害。注意：为了避免每帧都对同一个敌人造成伤害，
                        // 你可能需要一个机制（例如一个set）来跟踪本轮攻击已经命中的敌人。
                        // 当前实现会每帧都造成伤害，直到敌人死亡或攻击结束。
                        std::cout << "敌人 " << (*it).get() << " 在卡比的攻击范围内，受到伤害！" << std::endl;
                        (*it)->takeDamage(20); // 敌人受到 20 点伤害

                        if (!(*it)->getIsAlive()) {
                            it = enemiesPtr->erase(it); // 如果敌人死亡，从列表中移除
                            continue; // 移除后，继续检查下一个敌人
                        }
                    }
                }
                ++it; // 移动到下一个敌人
            }
        }
    }
    // --- 非攻击状态下的动画和逻辑 ---
    else { // 如果当前没有在攻击
        if (crouchPressed) {
            sprite.setTextureRect(crouchingRect); // 显示下蹲动画帧
            // 下蹲时可能需要调整精灵位置以保持底部对齐
            // sprite.setPosition(position.x, position.y + (standingRect.height - crouchingRect.height));
        }
        else {
            // 默认显示站立动画帧
            sprite.setTextureRect(standingRect);
            // 恢复精灵位置（如果之前因下蹲或攻击而调整过）
            // sprite.setPosition(position);
        }
    }

    // 根据朝向翻转精灵（如果 facingRight 为 false，水平翻转精灵）
    sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f });
    sprite.setScale(facingRight ? sf::Vector2f(1.f, 1.f) : sf::Vector2f(-1.f, 1.f));

    // --- 重力模拟 (简单实现) ---
    if (!isGrounded) {
        velocity.y += 980.f * static_cast<float>(0.016f); // 简单重力加速度 (例如 980 像素/秒^2，0.016 是 deltaTime 示例)
    }

    // --- 简单地面碰撞检测 (防止掉出屏幕底部) ---
    // 假设地面 Y 坐标为 500 像素，并且卡比的高度是 50 像素（为了让底部对齐地面）
    // 或者用 sprite.getGlobalBounds().height 动态获取高度
    float groundLevel = 500.f - sprite.getGlobalBounds().size.y; // 地面Y坐标 - 卡比高度
    if (position.y >= groundLevel) {
        position.y = groundLevel; // 将卡比固定在地面上
        velocity.y = 0.f;         // 垂直速度归零
        isGrounded = true;        // 标记为在地面上
    }
}


// attack：触发卡比的攻击
void Kirby::attack(std::vector<std::shared_ptr<Enemy>>* enemies) {
    if (!isAttacking) { // 只有当前不在攻击状态时才能启动新的攻击
        isAttacking = true;
        attackTimer.restart(); // 重置攻击计时器，开始攻击动画计时
        std::cout << "Kirby 开始攻击！" << std::endl;
        // 可以在这里播放攻击音效或粒子效果
        // 将传入的敌人列表指针赋值给成员变量，以便 updateLogic 中使用
        enemiesPtr = enemies;
    }
    // 实际的敌人伤害检测和动画播放将在 updateLogic 中进行
}

// takeDamage：卡比受到伤害
void Kirby::takeDamage(int amount) {
    if (isInvincible) {
        std::cout << "Kirby 无敌，未受到伤害！" << std::endl;
        return; // 无敌状态下不受伤
    }

    health -= amount; // 受到固定 10 点伤害
    std::cout << "Kirby 受到伤害！当前生命值：" << health << std::endl;

    if (health <= 0) {
        health = 0; // 生命值不为负
        isAlive = false; // 卡比被击败
        std::cout << "Kirby 被击败了！游戏结束。" << std::endl;
        // 可以在这里触发死亡动画、游戏结束画面等
    }
    else {
        isInvincible = true;       // 进入无敌状态
        invincibilityTimer.restart(); // 重置无敌计时器
        std::cout << "Kirby 进入无敌状态，持续 " << invincibilityDuration << " 秒。" << std::endl;
    }
}

//判断与墙壁，地板的碰撞
void Kirby::handleObstacleCollisions(const std::vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        // 检查当前帧是否与障碍物发生重叠
        if (std::optional<sf::FloatRect> intersection = boundingBox.findIntersection(obstacle.getGlobalBounds())) {
            // 检查交集是否有效（宽度和高度都大于0）
            if (intersection->size.x > 0 && intersection->size.y > 0) {
                // 判断是水平重叠更小，还是垂直重叠更小
                // 沿着重叠量更小的方向将速度归零并推开
                if (intersection->size.x < intersection->size.y) {
                    // 水平方向的重叠更小，说明是水平碰撞
                    velocity.x = 0; // 停止水平移动

                    // 将Kirby推开，避免卡在障碍物中
                    if (boundingBox.position.x < obstacle.getGlobalBounds().position.x) {
                        // Kirby 在障碍物左侧，向左推开
                        position.x -= intersection->size.x;
                    }
                    else {
                        // Kirby 在障碍物右侧，向右推开
                        position.x += intersection->size.x;
                    }
                    std::cout << "Horizontal collision: velocity.x = 0." << std::endl;
                }
                else {
                    // 垂直方向的重叠更小，说明是垂直碰撞
                    velocity.y = 0; // 停止垂直移动

                    // 将Kirby推开，避免卡在障碍物中
                    if (boundingBox.position.y < obstacle.getGlobalBounds().position.y) {
                        // Kirby 在障碍物上方，向上推开
                        position.y -= intersection->size.y;
                        isGrounded = true; // 垂直向下撞击到物体，视为着地
                    }
                    else {
                        // Kirby 在障碍物下方，向下推开
                        position.y += intersection->size.y;
                    }
                    std::cout << "Vertical collision: velocity.y = 0." << std::endl;
                }

                // 更新精灵位置和碰撞框，以反映推开后的新位置
                sprite.setPosition(position);
                boundingBox = sprite.getGlobalBounds();
            }
        }
    }
}

// handleGoalAndItemCollisions 
void Kirby::handleGoalAndItemCollisions(Goal& goal, std::vector<BoostItem>& boostItems) {
    if (goal.getIsAlive() && boundingBox.findIntersection(goal.getGlobalBounds())) {
        std::cout << "Kirby reached the goal! Game Won!" << std::endl;
        goal.setAlive(false);
    }

    for (auto it = boostItems.begin(); it != boostItems.end(); ) {
        if (it->getIsAlive() && boundingBox.findIntersection(it->getGlobalBounds())) {
            it->handleCollision(*this);
            it = boostItems.erase(it);
        }
        else {
            ++it;
        }
    }
}

