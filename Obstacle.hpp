#pragma once


#include "Entity.hpp" // 确保包含基类头文件
#include <iostream>   // 用于调试输出

class Obstacle : public Entity {

public:
    // 构造函数
    // Obstacle 通常是静态的，所以可能不需要 velocity 参数，但为了匹配基类构造函数，我们保留它
    Obstacle(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect, bool solid = true);

    // 重写纯虚函数
    void setSpriteTextureRect(const sf::IntRect& rect) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

};

