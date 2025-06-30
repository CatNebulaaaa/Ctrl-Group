#include "Game.h"
#include <iostream>
#include "PlayState.h"

Game::Game(Player& player)//设置的是player的纹理
    : map(), currentplaystate() {
    
}

void Game::loadLevel(PlayState& currentplaystate1, Player& player) {
    currentplaystate = currentplaystate1;

    // 示例：加载地图
    switch (currentplaystate.level) {
    case 1:
        map.loadFromFile("image/assets/forest_map.txt");
        player.setposition(100, 300); // 设置起点
        break;
    case 2:
        map.loadFromFile("image/assets/castle_map.txt");
        player.setposition(100, 300); // 设置起点  
        break;
    case 3: 
        map.loadFromFile("image/assets/snow_map.txt");
        player.setposition(100, 250);
        break;
        /*
    case 4:
        map.loadFromFile("mapEndless.txt");
        player.setposition(100, 200);
        break;*/
    default: break;
    }
    
    // 可以加载音乐、敌人、道具等内容
    /*
    enemyManager.clear();
    enemyManager.spawnEnemiesForLevel(level);*/
}

void Game::savegamestate()
{
    saveGame(currentplaystate);
}
/*
//放在main函数的按键之下
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    PlayState playtruestate;
    playtruestate.level = currentLevel;
    playtruestate.playerX = player.getPosition().x;
    playtruestate.playerY = player.getPosition().y;
    playtruestate.health = player.getHealth();
    playtruestate.lives = player.getLives();
    playtruestate.score = player.getScore();
    game.savegamestate;//要在前面创建一个Game类
    std::cout << "存档成功！\n";
}
*/

void Game::draw(sf::RenderWindow& window, Player& player)
{
    map.draw(window);
    //player.draw(window);
}

void Game::update(Player& player) {
    // 1. 处理玩家输入（例如方向键、跳跃等）
    player.handleInput();

    // 2. 更新玩家状态（如移动、动画等）
    sf::Clock clock;
    float deltaTime = clock.restart().asSeconds();
    player.update(deltaTime); // 如果有地图碰撞，用 map 判断

    // 4. 判断是否通关
    if (isLevelFinished(player)) {
        currentplaystate.level += 1;
        std::cout << "恭喜通关，进入下一关 Level " << currentplaystate.level << std::endl;
        loadLevel(currentplaystate, player);
    }

    // 5. 可选：判断游戏是否失败
    if (player.getposition().y > 1000) { // 玩家掉下去（死亡）
        std::cout << "你掉下去了！重新开始当前关卡。" << std::endl;
        loadLevel(currentplaystate, player);
    }
}
