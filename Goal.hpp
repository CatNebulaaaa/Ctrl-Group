#pragma once

#include "Entity.hpp" 
#include <iostream>   

class Goal : public Entity {


public:
    // 构造函数
    Goal(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect);

    // 重写纯虚函数
    void setSpriteTextureRect(const sf::IntRect& rect) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    
};