#pragma once
#include "TileMap.h"
#include <SFML/Graphics.hpp>  // 一定要包含这个
#include "GameLevel.h"
#include "PlayState.h"
#include "Kirby.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "BoostItem.hpp"
#include "MainMenu.h"
#include "GameOverScene.h"
#include <vector>
#include <map>
#include <SFML/Audio.hpp>

class Game {
public:
    Game(Kirby& player);
    void loadLevel(PlayState& currentplaystate1, Kirby& player, bool resetToSpawnPoint = true);//加载地图
    void update(Kirby& player, float deltaTime, sf::RenderWindow& window);//更新玩家动作，位置等信息，以及是否成功/失败
    void draw(sf::RenderWindow& window, Kirby& player);//将地图和玩家画上去
    TileMap& getTileMap();

    void createBoostItems();
    void createEnemies(); // 创建敌人
    bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);

    bool gameOver = false;

    void saveGameState(Kirby& player); // 存档方法
    bool loadGameState(Kirby& player); // 读档方法

    void loadSounds();  //加载音频资源
private:
    TileMap map;
    sf::Texture tileset;    // 瓦片集纹理（贴图）
    PlayState currentplaystate;
    sf::Vector2f spawnPoint;

    //创建增益道具
    sf::Texture healTexture;
    sf::Texture damageUpTexture;
    sf::Texture invincibleTexture;
    sf::Texture hasteTexture;
    std::vector<BoostItem> boostItems;
    sf::Texture boostTexture;

    sf::Texture enemyTexture;   // 敌人纹理
    std::vector<std::unique_ptr<Enemy>> enemies; // 使用智能指针存敌人对象

    sf::SoundBuffer pengBuffer;
    sf::Sound pengSound;

    // 添加设置按钮纹理和精灵
    sf::Texture settingsButtonTexture;
};