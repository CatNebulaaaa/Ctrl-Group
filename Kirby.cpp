#include "Kirby.h"

// 构造函数，初始化角色的基本属性和状态
Kirby::Kirby() {
    body.setSize(sf::Vector2f(16, 16)); // 设置角色矩形碰撞体大小，和地图格子大小对应
    body.setFillColor(sf::Color::Black); // 角色颜色设置为黑色，便于区分
    velocity = sf::Vector2f(0.f, 0.f); // 初始速度为零
    onGround = true;  // 初始时角色在地面上
    climbing = false;  // 初始时角色不在爬梯子状态
    dead = false;      // 初始时角色未死亡
    tileMap = nullptr; // 地图指针初始化为空，之后通过setMap设置
}

// 绑定地图指针，方便角色后续查询地图格子类型
void Kirby::setMap(TileMap* map) {
    tileMap = map;
}

// 设置角色位置（左上角坐标）
void Kirby::setPosition(float x, float y) {
    body.setPosition({ x, y });
    velocity = sf::Vector2f(0.f, 0.f); // 位置变了，速度归零

    fixInitialPosition(); // 调用自动落地函数，修正位置
}

// 处理键盘输入，控制角色水平移动，爬梯子，跳跃
void Kirby::handleInput() {
    if (dead) return; // 如果角色死亡，停止处理输入

    // 左键按下，向左移动，速度设为-60
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        velocity.x = -60.f;

    // 右键按下，向右移动，速度设为60
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        velocity.x = 60.f;

    // 查询当前角色所处格子类型，判断是否在梯子上
    TileType type = getCurrentTileType();

    if (type == TileType::ladder) {  // 如果在梯子上
        climbing = true;              // 设为爬梯子状态
        velocity.y = 0.f;             // 爬梯子时垂直速度先置零，避免重力影响

        // 上键按下，向上爬梯子，速度为-40
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            velocity.y = -40.f;
        // 下键按下，向下爬梯子，速度为40
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            velocity.y = 40.f;
    }
    else {
        climbing = false;  // 不在梯子上，取消爬梯子状态

        // 如果角色站在地面上，且空格键按下，触发跳跃，垂直速度设为-150（向上）
        if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            velocity.y = -150.f;
    }
}

// 应用重力，改变垂直速度（除非正在爬梯子）
void Kirby::applyGravity(float dt) {
    if (climbing==0 && isOnGround() == 0) {
        velocity.y += 400.f * dt; // 重力加速度为400，乘以时间间隔dt，速度累加
    }
}

// 每帧更新角色状态
void Kirby::update(float dt) {
    if (dead) return;

    handleInput();     // 更新 velocity.x / velocity.y
    applyGravity(dt);  // 模拟重力，修改 velocity.y

    body.move(velocity * dt); //实际移动角色

    checkCollisions(); // 检查地图碰撞、修正位置（落地、掉水等）
}


// 绘制角色矩形到窗口
void Kirby::draw(sf::RenderWindow& window) {
    window.draw(body);
}

// 查询角色是否死亡
bool Kirby::isDead() const {
    return dead;
}

// 获取角色中心所在格子类型，用于判断脚下或当前位置环境
TileType Kirby::getCurrentTileType() {
    if (!tileMap) return TileType::empty; // 没绑定地图，默认空格子

    // 计算角色中心点对应的地图格子行列索引
    int row = (int)((body.getPosition().y + body.getSize().y / 2) / tileMap->getTileHeight());
    int col = (int)((body.getPosition().x + body.getSize().x / 2) / tileMap->getTileWidth());

    // 返回该格子的类型（如地面、梯子、水等）
    return tileMap->getTileType(row, col);
}

// 判断角色脚底是否踩在地面顶层
bool Kirby::isOnGround() {
    if (!tileMap) return false; // 未绑定地图，视为不在地面

    float footY = body.getPosition().y + body.getSize().y; // 计算角色脚底y坐标
    int footRow = (int)(footY / tileMap->getTileHeight()); // 计算脚底对应地图格子行号

    // 计算脚底左右边对应的列号（避免角色横跨多个格子）
    int leftCol = (int)(body.getPosition().x / tileMap->getTileWidth());
    int rightCol = (int)((body.getPosition().x + body.getSize().x) / tileMap->getTileWidth());

    // lambda函数判断某格子是否为地面顶层
    // 条件：该格是地面格子，且它上一格不是地面，表示顶层
    auto isGroundTop = [&](int r, int c) {
        if (tileMap->getTileType(r, c) != TileType::ground)
            return false;
        if (r == 0) // 第一行没有上一格，直接认为是顶层
            return true;
        return tileMap->getTileType(r - 1, c) != TileType::ground;
        };

    // 只要脚底左边或右边的格子是地面顶层，就认为角色踩在地面上
    return (isGroundTop(footRow, leftCol) || isGroundTop(footRow, rightCol));
}

// 碰撞检测与处理
void Kirby::checkCollisions() {
    if (!tileMap) return; // 未绑定地图，跳过

    // 先判断是否踩在地面顶层
    if (isOnGround()) {
        onGround = true;  // 标记角色踩在地面

        // 计算脚底所在行，获取该行地面高度
        int footRow = (int)((body.getPosition().y + body.getSize().y) / tileMap->getTileHeight());
        float groundY = footRow * tileMap->getTileHeight();

        // 调整角色y，使脚底正好位于地面上，防止角色下沉进入地面
        body.setPosition({ body.getPosition().x, groundY - body.getSize().y });

        // 如果角色有向下速度（下落），则清零，停止下落
        if (velocity.y > 0) velocity.y = 0;
    }
    else {
        onGround = false; // 不在地面上
    }

    // 查询当前格子类型，处理特殊情况
    TileType type = getCurrentTileType();

    // 角色踩到水格，触发死亡状态
    if (type == TileType::water) {
        dead = true;
    }

    // 角色撞到障碍物
    if (type == TileType::obstacle) {
        // 简单处理：角色回退到上一帧位置，停止移动
        body.move(-velocity);
        velocity = sf::Vector2f(0.f, 0.f);
    }
}

// 返回角色当前左上角坐标
sf::Vector2f Kirby::getPosition() {
    return body.getPosition();
}

void Kirby::fixInitialPosition() {
    if (!tileMap) return;

    if (isOnGround()) {
        onGround = true;
        velocity.y = 0;
        return;
    }

    const float dt = 0.016f;
    const int maxIterations = 100;

    int count = 0;
    velocity.y = 0; // 避免继承上次的速度

    while (!isOnGround() && count < maxIterations) {
        applyGravity(dt);
        body.move(velocity * dt);
        checkCollisions(); // 这个可能没能设置onGround为true
        count++;
    }

    velocity.y = 0;

    if (isOnGround()) {
        onGround = true; //最后落地后确保设置上
    }
}


