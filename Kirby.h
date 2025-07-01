#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.h" // 地图类，包含地图数据和格子信息

class Kirby {
public:
    Kirby(); // 构造函数，初始化角色属性

    void setMap(TileMap* map);           // 绑定地图指针，用于查询地图格子类型
    void setPosition(float x, float y);  // 设置角色的初始位置
    void handleInput();                  // 处理键盘输入，改变角色速度
    void update(float dt);               // 每帧更新角色状态，dt为时间间隔
    void draw(sf::RenderWindow& window); // 绘制角色到窗口
    sf::Vector2f getPosition();          // 获取角色当前位置

    bool isDead() const;                 // 查询角色是否死亡

    // 新增，判断角色脚底是否踩在地面顶层
    bool isOnGround();
    //自动落到地面上
    void fixInitialPosition();

private:
    sf::RectangleShape body;             // 角色的图形表示，用矩形模拟碰撞体
    sf::Vector2f velocity = { 0.f,0.f };               // 角色当前速度（水平和垂直）
    bool onGround=1;                      // 角色是否站在地面（脚底踩着地表）
    bool climbing=0;                      // 角色是否处于爬梯子状态
    bool dead=0;                         // 角色是否处于死亡状态

    TileMap* tileMap;                  // 指向地图实例的指针，方便查询格子类型

    void applyGravity(float dt);        // 给角色应用重力，影响垂直速度
    void checkCollisions();             // 碰撞检测，处理角色与地图的交互
    TileType getCurrentTileType();      // 获取角色中心所在格子的类型    
};
