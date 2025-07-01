#include "Game.h"
#include <iostream>
#include "PlayState.h"

Game::Game(Kirby& player)//设置的是player的纹理
    : map(), currentplaystate() {

}

void Game::loadLevel(PlayState& currentplaystate1, Kirby& player) {
    currentplaystate = currentplaystate1;

    switch (currentplaystate.level) {
    case 1:
        if (!tileset.loadFromFile("image/map1.png")) {
            std::cerr << "地图贴图 image/map1.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }
        player.setPosition(100, 250);
        break;
    case 2:
        if (!tileset.loadFromFile("image/map1.png")) {
            std::cerr << "地图贴图 image/map1.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }
        player.setPosition(100, 250);
        break;
    case 3:
        if (!tileset.loadFromFile("image/map1.png")) {
            std::cerr << "地图贴图 image/map1.png 加载失败！" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "地图文件 map1.tmx 加载失败！" << std::endl;
            return;
        }
        player.setPosition(100, 250);
        break;
        /*
    case 4:
        map.loadFromFile("mapEndless.txt");
        player.setposition(100, 200);
        break;*/
    default: break;
    }
    player.setMap(&map);
    player.fixInitialPosition(); 
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

void Game::draw(sf::RenderWindow& window, Kirby& player)
{
    window.clear();
    window.draw(map);
    player.draw(window);
    window.display();
}

void Game::update(Kirby& player) {
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
    if (player.getPosition().y > 1000) { // 玩家掉下去（死亡）
        std::cout << "你掉下去了！重新开始当前关卡。" << std::endl;
        loadLevel(currentplaystate, player);
    }
}

TileMap& Game::getTileMap() {
    return map;
}
