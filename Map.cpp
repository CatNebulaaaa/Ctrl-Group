#include "Map.h"
#include <fstream>
#include <sstream>
#include <iostream>

Map::Map(): tileSprite(tileset) {}

//加载地图数据的函数（从 .txt 文件中读取地形信息）
bool Map::loadFromFile(const std::string& filename) {
    // 加载贴图（图集），假设是 tileset.png
    if (!tileset.loadFromFile("image/assets/mapall.png")) {
        std::cerr << "无法加载 tileset.png！" << std::endl;
        return false;
    }
    tileSprite.setTexture(tileset);

    // 读取地图文本文件
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开地图文件：" << filename << std::endl;
        return false;
    }

    tiles.clear(); // 清空旧地图数据
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        int tile;
        while (ss >> tile) {
            row.push_back(tile);
        }
        tiles.push_back(row);
    }

    std::cout << "地图加载完成：" << filename << std::endl;
    return true;
}

//将地图绘制到窗口 window 上
void Map::draw(sf::RenderWindow& window) {
    if (tileset.getSize().x == 0 || tileSize == 0)
        return; // 防止除以0或贴图未加载

    int tilesPerRow = tileset.getSize().x / tileSize; // 每行有多少个tile

    for (size_t y = 0; y < tiles.size(); ++y) {
        for (size_t x = 0; x < tiles[y].size(); ++x) {
            int tileID = tiles[y][x];
            if (tileID < 0) continue; // -1 表示空地

            // 计算贴图中的坐标
            int tu = tileID % tilesPerRow; // tile 在第几列
            int tv = tileID / tilesPerRow; // tile 在第几行

            // 裁剪出图集中对应的砖块
            tileSprite.setTextureRect(sf::IntRect({ tu * tileSize, tv * tileSize }, { tileSize, tileSize }));

            // 设置地图上绘制的位置
            tileSprite.setPosition({ static_cast<float>(x * tileSize), static_cast<float>(y * tileSize) });

            // 画出来
            window.draw(tileSprite);
        }
    }
}
