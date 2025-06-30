#include <fstream>
#include "PlayState.h"

void saveGame(const PlayState& playstate) {
    std::ofstream outFile("save.txt"); // 存在程序运行目录下
    if (outFile.is_open()) {
        outFile << playstate.level << "\n";
        outFile << playstate.playerX << " " << playstate.playerY << "\n";
        outFile << playstate.health << "\n";
        outFile << playstate.lives << "\n";
        outFile << playstate.score << "\n";
        outFile.close();
    }
}

bool loadGame(PlayState& playstate) {
    std::ifstream inFile("save.txt");
    if (inFile.is_open()) {
        inFile >> playstate.level;
        inFile >> playstate.playerX >> playstate.playerY;
        inFile >> playstate.health;
        inFile >> playstate.lives;
        inFile >> playstate.score;
        inFile.close();
        return true;
    }
    return false; // 文件不存在
}

bool isLevelFinished(Player& player) {
    // 如果玩家 X 坐标大于 800，则认为到达终点（你可以自己设置这个值）
    return player.getposition().x >= 800;
}