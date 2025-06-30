#pragma once
#include "Player.h"

struct PlayState {
    int level;          // 当前关卡编号
    float playerX;      // 主角位置X
    float playerY;      // 主角位置Y
    int health;         // 主角血量
    int lives;          // 主角生命数
    int score;          // 当前分数  
};
bool isLevelFinished(Player& player);
void saveGame(const PlayState& playstate);
bool loadGame(PlayState& playstate);

