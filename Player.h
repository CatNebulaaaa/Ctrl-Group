#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Texture &texture);

    void handleInput();           // 处理键盘输入
    void load(sf::Texture& texture, sf::RenderWindow& window);  // 加载贴图等

    void setposition(int x, int y);//设置位置
    void setposition( float x, float y );
    void setHealth(int &health);
    void setScore(int getscore);

    sf::Vector2f getposition();
    sf::FloatRect getBounds() const; // 返回碰撞箱

    void update(float deltaTime); // 更新位置、速度等
    void draw(sf::RenderWindow& window); // 绘制角色
    
    void takeDamage(int dmg);        // 扣血
    bool isAlive() const;

private:
    sf::Sprite sprite;
    
    sf::Vector2f velocity;//玩家当前的移动速度向量
    sf::Vector2f position;

    float speed;
    float jumpForce;
    bool isOnGround;

    int health;
    int maxHealth;

    int score;

    void applyGravity(float deltaTime);
    void checkBounds(); // 限制在窗口内或地图内
};

#endif // PLAYER_H
