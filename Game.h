#pragma once
#include "TileMap.h"
#include <SFML/Graphics.hpp>  // 一定要包含这个
#include "GameLevel.h"
#include "PlayState.h"
#include "Kirby.h"

class Game {
public:
    Game(Kirby& player);
    void loadLevel(PlayState& currentplaystate1, Kirby& player);//加载地图
    void update(Kirby& player);//更新玩家动作，位置等信息，以及是否成功/失败
    void draw(sf::RenderWindow& window, Kirby& player);//将地图和玩家画上去
    void savegamestate();//保存游戏进度
    TileMap& getTileMap();


private:
    TileMap map;
    sf::Texture tileset;    // 瓦片集纹理（贴图）
    PlayState currentplaystate;

};