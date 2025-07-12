#include <fstream>
#include "../include/PlayState.h"

void saveGame(const PlayState& playstate) {
    std::ofstream outFile("save.txt"); // 存在程序运行目录下
    if (outFile.is_open()) {
        outFile << playstate.level << "\n";
        outFile << playstate.playerX << " " << playstate.playerY << "\n";
        outFile << playstate.health << "\n";
        outFile.close();
    }
}

bool loadGame(PlayState& playstate) {
    std::ifstream inFile("save.txt");
    if (inFile.is_open()) {
        inFile >> playstate.level;
        inFile >> playstate.playerX >> playstate.playerY;
        inFile >> playstate.health;
        inFile.close();
        return true;
    }
    return false; // 文件不存在
}

bool isLevelFinished(Kirby& player, TileMap* tileMap) {
    if (!tileMap) return false;

    sf::Vector2f pos = player.getPosition();
    sf::Vector2f size = player.getSize();

    float midX = pos.x + size.x / 2.f;
    float midY = pos.y + size.y/2.f;

    int tileX = static_cast<int>(midX) / tileMap->getTileWidth();
    int tileY = static_cast<int>(midY) / tileMap->getTileHeight();

    TileType tile = tileMap->getTileType(tileY, tileX);
    return tile == TileType::exit;
}

