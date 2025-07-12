#pragma once
#include "Kirby.h"
#include "Enemy.h"
#include "BoostItem.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>   //引入文件系统库
#include <iostream>
#include "Constants.h"


// PlayState.h
#pragma once
#include <vector>
// Game.h
struct PlayState {
    int level = 1;
    float playerX = 0.f;
    float playerY = 0.f;
    int health = 100;
    bool isInvincible = false;
    float hurtCooldown = 0.f;
};
// 游戏逻辑函数
bool isLevelFinished(Kirby& player, TileMap* tileMap);