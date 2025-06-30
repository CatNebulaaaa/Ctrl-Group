#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map {
public:
    Map();
    bool loadFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::vector<int>> tiles; // 地图的格子数据（每个 int 是 tile ID）,表示一个二维数组，存地图的格子
    sf::Texture tileset;                 // 所有砖块贴图
    sf::Sprite tileSprite;              // 用于绘制单个砖块

    int tileSize = 32;                  // 每个格子的像素大小（32x32）
};

