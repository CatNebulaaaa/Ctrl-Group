#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.h" // 地图类，包含地图数据和格子信息
#include "Bullet.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <stdexcept> // 用于异常处理

class Kirby {
public:
    Kirby(); // 构造函数，初始化角色属性

    void setMap(TileMap* map);           // 绑定地图指针，用于查询地图格子类型
    void setPosition(float x, float y);  // 设置角色的初始位置
    void handleInput();                  // 处理键盘输入，改变角色速度
    void update(float dt,int gamelevel=1);               // 每帧更新角色状态，dt为时间间隔
    void draw(sf::RenderWindow& window); // 绘制角色到窗口
    sf::Vector2f getPosition();          // 获取角色当前位置
    sf::Vector2f getSize() const;

    int getHealth() const { return health; }
    void takeDamage(int amount) 
    {
        health -= amount;
        if (health <= 0) {
            dead = true;
            health = 0;
        }
    }
    float hurtCooldown = 0.f; // 受伤冷却
    void sethealth(int amount)
    {
        health = amount;
    }
    void attack();
    bool isDead() const;                 // 查询角色是否死亡
    void setLives(int live)
    {
        dead = !live;
    }
    // 新增，判断角色脚底是否踩在地面顶层
    bool isOnGround();
    bool isOnLadder() const;
    //自动落到地面上
    void fixInitialPosition();

    bool isOnWaterSurface(); // 判断是否正下方某处是水
    TileType getCurrentTileType();

    sf::FloatRect getGlobalBounds() const {
        return body.getGlobalBounds();
    }

    const std::vector<Bullet>& getBullets() const { return bullets; }
    std::vector<Bullet>& getBullets() { return bullets; }
    friend class BoostItem;

    bool hasAttackBoost = false;
    sf::Clock attackBoostTimer;

    bool isInvincible = false;
    sf::Clock invincibilityTimer;

    bool hasSpeedBoost = false;
    sf::Clock speedBoostTimer;

    void loadSounds();  //加载音频资源
private:
    std::array<sf::Texture, 7> runTextures = {
        sf::Texture("assets/image/kirbyImage/kirby1.png"), sf::Texture("assets/image/kirbyImage/kirby2.png"), sf::Texture("assets/image/kirbyImage/kirby3.png"),
        sf::Texture("assets/image/kirbyImage/kirby4.png"), sf::Texture("assets/image/kirbyImage/kirby5.png"), sf::Texture("assets/image/kirbyImage/kirby6.png"),
        sf::Texture("assets/image/kirbyImage/kirby7.png")
    };
    sf::Texture texture_zero;
    sf::Sprite body;
    sf::Vector2f velocity = { 0.f,0.f };               // 角色当前速度（水平和垂直）
    bool onGround;                      // 角色是否站在地面（脚底踩着地表）
    bool climbing;                      // 角色是否处于爬梯子状态
    bool dead;                         // 角色是否处于死亡状态
    bool iswin;
    TileMap* tileMap;                  // 指向地图实例的指针，方便查询格子类型

    void applyGravity(float dt);        // 给角色应用重力，影响垂直速度
    void checkCollisions();             // 碰撞检测，处理角色与地图的交互
    // 加载纹理的方法
    void loadTextures();

    int currentFrame = 0;
    float frameTime = 0.1f; // 每 0.1 秒切换一帧
    float frameDuration = 0.08f;
    bool isRunning = false;
    bool facingRight = true;
    float scaleFactor = 0.075f; // 添加缩放因子

    int health = 100;
    int maxHealth = 100;
    std::vector<Bullet> bullets;
    float bulletCooldown = 0.f;

    sf::SoundBuffer attackBuffer;
    sf::Sound attackSound;
};
