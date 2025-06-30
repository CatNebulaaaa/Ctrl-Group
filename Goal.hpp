#pragma once

#include "Entity.hpp" 
#include <iostream>   

class Goal : public Entity {


public:
    // ���캯��
    Goal(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect);

    // ��д���麯��
    void setSpriteTextureRect(const sf::IntRect& rect) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    
};