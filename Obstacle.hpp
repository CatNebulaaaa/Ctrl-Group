#pragma once


#include "Entity.hpp" // ȷ����������ͷ�ļ�
#include <iostream>   // ���ڵ������

class Obstacle : public Entity {

public:
    // ���캯��
    // Obstacle ͨ���Ǿ�̬�ģ����Կ��ܲ���Ҫ velocity ��������Ϊ��ƥ����๹�캯�������Ǳ�����
    Obstacle(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect, bool solid = true);

    // ��д���麯��
    void setSpriteTextureRect(const sf::IntRect& rect) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

};

