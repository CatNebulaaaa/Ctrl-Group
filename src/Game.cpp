#include "../include/Game.h"
#include <iostream>
#include "../include/PlayState.h"
#include "../include/BulletManager.h"
#include "../include/Setting.h"

Game::Game(Kirby& player)//设置的是player的纹理
    : map(), currentplaystate(), pengSound(pengBuffer){
    if (!healTexture.loadFromFile("assets/image/item.png")) {
        std::cerr << "治疗道具纹理加载失败!" << std::endl;
    }
    if (!damageUpTexture.loadFromFile("assets/image/item.png")) {
        std::cerr << "攻击提升道具纹理加载失败!" << std::endl;
    }
    if (!invincibleTexture.loadFromFile("assets/image/item.png")) {
        std::cerr << "无敌道具纹理加载失败!" << std::endl;
    }
    if (!hasteTexture.loadFromFile("assets/image/item.png")) {
        std::cerr << "加速道具纹理加载失败!" << std::endl;
    }
    //加载敌人纹理
    if (!enemyTexture.loadFromFile("assets/image/enemy.png")) {
        std::cerr << "敌人纹理加载失败!" << std::endl;
    }
    //设置按钮
    if (!settingsButtonTexture.loadFromFile("assets/image/settings.png")) {
        std::cerr << "Failed to load settings button texture!" << std::endl;
    }
    spawnPoint = sf::Vector2f(20, 10);
    loadSounds();
}
void Game::createBoostItems() {
    boostItems.clear();

    // 确保不会创建过多对象
    if (boostItems.capacity() > MAX_ITEMS) {
        boostItems.shrink_to_fit();
    }
    // 根据关卡创建不同道具
    switch (currentplaystate.level) {
    case 1:
        boostItems.push_back(BoostItem(sf::Vector2f(100, 300), healTexture, BoostType::HEAL));
        boostItems.push_back(BoostItem(sf::Vector2f(500, 300), damageUpTexture, BoostType::INVINCIBLE));
        break;
    case 2:
        boostItems.push_back(BoostItem(sf::Vector2f(300, 400), invincibleTexture, BoostType::INVINCIBLE));
        boostItems.push_back(BoostItem(sf::Vector2f(600, 200), hasteTexture, BoostType::HEAL));
        break;
    case 3:
        boostItems.push_back(BoostItem(sf::Vector2f(300, 420), healTexture, BoostType::HEAL));
        boostItems.push_back(BoostItem(sf::Vector2f(600, 170), hasteTexture, BoostType::INVINCIBLE));
        break;
    case 4:
        boostItems.push_back(BoostItem(sf::Vector2f(100, 300), healTexture, BoostType::HEAL));
        boostItems.push_back(BoostItem(sf::Vector2f(500, 300), damageUpTexture, BoostType::INVINCIBLE));
        break;
    default:break;
    }

    // 设置尺寸（如果需要）
    for (auto& item : boostItems) {
        // 获取纹理尺寸
        sf::Vector2u textureSize = item.getTexture().getSize();

        // 设置精灵尺寸（例如统一缩放到40*40）
        float scaleX = 40.0f / textureSize.x;
        float scaleY = 40.0f / textureSize.y;
        item.setScale(scaleX, scaleY);
    }
}
void Game::loadSounds()
{
    if (!pengBuffer.loadFromFile("assets/sound/attck.wav")) {
        std::cerr << "无法加载点击音效！" << std::endl;
    }
    else {
        pengSound.setBuffer(pengBuffer);
    }
}
void Game::loadLevel(PlayState& currentplaystate1, Kirby& player, bool resetToSpawnPoint) {
    currentplaystate = currentplaystate1;

    // 加载地图（原有代码不变）
    switch (currentplaystate.level) {
    case 1:
        if (!tileset.loadFromFile("assets/image/map1.png")) {
            std::cerr << "地图贴图 image/map1.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("assets/image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }
        break;
    case 2:
        if (!tileset.loadFromFile("assets/image/map2.png")) {
            std::cerr << "地图贴图 image/map2.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("assets/image/map2.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }
        break;
    case 3:
        if (!tileset.loadFromFile("assets/image/map3.png")) {
            std::cerr << "地图贴图 image/map1.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("assets/image/map3.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }

        break;
    case 4:
        if (!tileset.loadFromFile("assets/image/map1.png")) {
            std::cerr << "地图贴图 image/map1.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("assets/image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }
        break;
    default: break;
    }

    // 如果是重置关卡，使用初始出生点
    if (resetToSpawnPoint) {
        player.setPosition(20, 10);
    }

    createBoostItems();
    player.setMap(&map);
    createEnemies();
}

void Game::draw(sf::RenderWindow& window, Kirby& player)
{
    window.clear();
    window.draw(map);

    // 绘制道具
    for (auto& item : boostItems) {
        if (item.getIsAlive())
            item.draw(window);
    }
    for (auto& enemy : enemies) {
        if (enemy->getIsAlive())
            enemy->draw(window);
    }
    sf::Sprite settingsButton(settingsButtonTexture);
    settingsButton.setPosition({ 700.f, 0.f }); // 假设窗口宽度是 800
    settingsButton.setScale({ 0.15, 0.15 });
    // 画设置按钮
    window.draw(settingsButton);
    // 检测点击设置按钮
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                if (settingsButton.getGlobalBounds().contains(mousePos))
                {
                    std::cout << "设置按钮被点击了！" << std::endl;
                    // 可以改为弹出设置菜单，或切换场景
                    sf::RenderWindow window1(sf::VideoMode({ 800, 600 }), "Setting");
                    //加载窗口背景纹理
                    sf::Texture backgroundTexture;//背景纹理
                    if (!backgroundTexture.loadFromFile("assets/image/setting.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
                    {
                        return; // 加载失败
                    }
                    sf::Sprite backgroundSprite(backgroundTexture);//背景精灵
                    sf::Vector2u windowSize = window.getSize();
                    sf::Vector2u textureSize = backgroundTexture.getSize();
                    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
                    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
                    backgroundSprite.setScale({ scaleX, scaleY });

                    //创建了一个菜单类
                    MainMenu menu(backgroundSprite);
                    Setting settingmap(window1);
                    settingmap.run(window1, menu, window);
                }
            }
        }
    }
    player.draw(window);
    window.display();
}

void Game::update(Kirby& player, float deltaTime, sf::RenderWindow &window) {
    if (gameOver) return;  // 如果游戏已经结束，不再更新

    if (currentplaystate.level == 4)
    {
        if(!player.isInvincible)    std::cout << "卡比处于无敌关卡！" << std::endl;
        player.isInvincible = true;    
    }
    // 1. 处理玩家输入（例如方向键、跳跃等）
    player.handleInput();

    // 2. 更新玩家状态（如移动、动画等）
    player.update(deltaTime, currentplaystate.level); // 如果有地图碰撞，用 map 判断

    // 更新道具
    for (auto& item : boostItems) {
        item.update(deltaTime);
        item.handleCollision(player); // 检测与玩家的碰撞
    }

    // 移除已被收集的道具
    boostItems.erase(std::remove_if(boostItems.begin(), boostItems.end(),
        [](const BoostItem& item) { return !item.getIsAlive(); }),
        boostItems.end());

    // 更新敌人
    for (auto& enemy : enemies) {
        if (enemy->getIsAlive())
            enemy->update(deltaTime);

        // 子弹与敌人碰撞检测
        for (auto& bullet : player.getBullets()) {
            if (enemy->getIsAlive() && intersects(bullet.getBounds(), enemy->getGlobalBounds())) {
                enemy->takeHit();        // 敌人受击
                bullet.dead = true;      // 子弹死亡，标记为false或dead
            }
        }
    }

    // 删除死亡敌人
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->getIsAlive(); }),
        enemies.end());
    // 更新卡比受伤冷却计时
    if (player.hurtCooldown > 0.f) {
        player.hurtCooldown -= deltaTime;
    }

    // 敌人与卡比碰撞
    for (auto& enemy : enemies) {
        if (enemy->getIsAlive() &&
            intersects(player.getGlobalBounds(), enemy->getGlobalBounds()) &&
            player.hurtCooldown <= 0.f) {
            // 如果卡比当前是无敌状态，跳过受伤
            if (player.isInvincible) {
                std::cout << "卡比无敌中，未受到伤害。\n";
                continue;
            }
            pengSound.play();
            player.takeDamage(10);
            player.hurtCooldown = 1.0f; // 受伤后冷却 1 秒
            std::cout << "卡比受伤了！当前血量: " << player.getHealth() << std::endl;
        }
    }
    // 4. 判断是否通关
    if (isLevelFinished(player, &map)) {
        currentplaystate.level += 1;
        std::cout << "恭喜通关，进入下一关 Level " << currentplaystate.level << std::endl;
        if(currentplaystate.level<=4)   loadLevel(currentplaystate, player);
        else
        {
            window.close();
            // 进入结局界面并标记游戏结束
            gameOver = true;  // 关键：设置标志
            //设置窗口
            sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Kirby Adventure");
            //加载窗口背景纹理
            sf::Texture backgroundTexture;//背景纹理
            if (!backgroundTexture.loadFromFile("assets/image/background.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
            {
                cout << "失败";
                return; // 加载失败
            }
            sf::Sprite backgroundSprite(backgroundTexture);//背景精灵
            sf::Vector2u windowSize = window.getSize();
            sf::Vector2u textureSize = backgroundTexture.getSize();
            float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
            float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
            backgroundSprite.setScale({ scaleX, scaleY });

            //创建了一个菜单类
            MainMenu menu(backgroundSprite);
            GameLevel gamelevel;
            GameOverScene overscene(window);
            overscene.run(window,gamelevel,menu);
        }
    }
    // 5. 可选：判断游戏是否失败
    if (player.getPosition().y > 600) { // 玩家掉下去（死亡）
        std::cout << "你掉下去了！重新开始当前关卡。" << std::endl;
        loadLevel(currentplaystate, player);
    }
    if (player.getPosition().y < 0 || player.getPosition().x>800 || player.getPosition().x < 0)
    {
        std::cout << "你不见了！重新开始当前关卡。" << std::endl;
        loadLevel(currentplaystate, player);
    }
    if (player.getHealth()<=0) { // 玩家死亡
        std::cout << "你被敌人攻击死亡。" << std::endl;
        player.sethealth(100);
        loadLevel(currentplaystate, player);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        saveGameState(player);
        std::cout << "游戏已存档！" << std::endl;
    }
}

TileMap& Game::getTileMap() {
    return map;
}

void Game::createEnemies() {
    enemies.clear(); // 清除旧敌人
    
    switch (currentplaystate.level) {
    case 1:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(100.f, 460.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    case 2:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(250.f, 50.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    case 3:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(200.f, 50.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    case 4:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(100.f, 460.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    default:
        break;
    }

    // 确保这里有设置缩放比例
    for (auto& enemy : enemies) {
        sf::Vector2u textureSize = enemy->getTexture().getSize();
        float scaleX = 40.0f / textureSize.x;
        float scaleY = 40.0f / textureSize.y;
        enemy->setScale(scaleX, scaleY);
    }
}

bool Game::intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.position.x < b.position.x + b.position.x &&
        a.position.x + a.size.x > b.position.x &&
        a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y;
}

// Game.cpp
void Game::saveGameState(Kirby& player) {
    // 更新当前游戏状态
    currentplaystate.playerX = player.getPosition().x;
    currentplaystate.playerY = player.getPosition().y;
    currentplaystate.health = player.getHealth();
    currentplaystate.isInvincible = player.isInvincible;
    currentplaystate.hurtCooldown = player.hurtCooldown;

    // 保存到文件
    std::ofstream outFile("savegame.dat", std::ios::binary);
    if (outFile) {
        // 保存基本游戏状态
        outFile.write(reinterpret_cast<const char*>(&currentplaystate), sizeof(currentplaystate));

        // 保存敌人数量
        size_t enemyCount = enemies.size();
        outFile.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));

        // 保存每个敌人的位置和存活状态
        for (const auto& enemy : enemies) {
            sf::Vector2f pos = enemy->getPosition();
            bool isAlive = enemy->getIsAlive();

            outFile.write(reinterpret_cast<const char*>(&pos.x), sizeof(pos.x));
            outFile.write(reinterpret_cast<const char*>(&pos.y), sizeof(pos.y));
            outFile.write(reinterpret_cast<const char*>(&isAlive), sizeof(isAlive));
        }

        // 保存道具数量
        size_t itemCount = boostItems.size();
        outFile.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));

        // 保存每个道具的存活状态
        for (const auto& item : boostItems) {
            bool isAlive = item.getIsAlive();
            outFile.write(reinterpret_cast<const char*>(&isAlive), sizeof(isAlive));
        }

        std::cout << "游戏已存档！" << std::endl;
    }
    else {
        std::cerr << "无法保存游戏！" << std::endl;
    }
}

// Game.cpp
bool Game::loadGameState(Kirby& player) {
    std::ifstream inFile("savegame.dat", std::ios::binary);
    if (!inFile) {
        std::cerr << "没有找到存档文件！" << std::endl;
        return false;
    }

    // 读取基本游戏状态
    inFile.read(reinterpret_cast<char*>(&currentplaystate), sizeof(currentplaystate));

    // 先加载地图
    loadLevel(currentplaystate, player, false); // false表示不重置到初始点

    // 设置玩家状态
    player.setPosition(currentplaystate.playerX, currentplaystate.playerY);
    player.sethealth(currentplaystate.health);
    player.isInvincible = currentplaystate.isInvincible;
    player.hurtCooldown = currentplaystate.hurtCooldown;

    // 读取敌人数量
    size_t enemyCount;
    inFile.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));

    // 重建敌人
    enemies.clear();
    for (size_t i = 0; i < enemyCount; ++i) {
        float x, y;
        bool isAlive;

        inFile.read(reinterpret_cast<char*>(&x), sizeof(x));
        inFile.read(reinterpret_cast<char*>(&y), sizeof(y));
        inFile.read(reinterpret_cast<char*>(&isAlive), sizeof(isAlive));

        if (isAlive) {
            auto enemy = std::make_unique<Enemy>(sf::Vector2f(x, y), enemyTexture, &map);
            // 设置敌人缩放比例
            sf::Vector2u textureSize = enemy->getTexture().getSize();
            float scaleX = 40.0f / textureSize.x;
            float scaleY = 40.0f / textureSize.y;
            enemy->setScale(scaleX, scaleY);
            enemies.push_back(std::move(enemy)); // 注意这里使用 std::move
        }
    }

    // 读取道具数量
    size_t itemCount;
    inFile.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount));

    // 重建道具状态
    boostItems.clear();
    createBoostItems(); // 先创建所有道具

    for (size_t i = 0; i < itemCount && i < boostItems.size(); ++i) {
        bool isAlive;
        inFile.read(reinterpret_cast<char*>(&isAlive), sizeof(isAlive));
        boostItems[i].setAlive(isAlive);
    }

    std::cout << "游戏已加载！" << std::endl;
    return true;
}