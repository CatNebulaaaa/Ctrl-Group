#include <fstream>
#include "PlayState.h"

void saveGame(const PlayState& playstate) {
    std::ofstream outFile("save.txt"); // ���ڳ�������Ŀ¼��
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
    return false; // �ļ�������
}

bool isLevelFinished(Player& player) {
    // ������ X ������� 800������Ϊ�����յ㣨������Լ��������ֵ��
    return player.getposition().x >= 800;
}