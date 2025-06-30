#pragma once
#include "Player.h"

struct PlayState {
    int level;          // ��ǰ�ؿ����
    float playerX;      // ����λ��X
    float playerY;      // ����λ��Y
    int health;         // ����Ѫ��
    int lives;          // ����������
    int score;          // ��ǰ����  
};
bool isLevelFinished(Player& player);
void saveGame(const PlayState& playstate);
bool loadGame(PlayState& playstate);

