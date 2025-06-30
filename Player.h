#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Texture &texture);

    void handleInput();           // �����������
    void load(sf::Texture& texture, sf::RenderWindow& window);  // ������ͼ��

    void setposition(int x, int y);//����λ��
    void setposition( float x, float y );
    void setHealth(int &health);
    void setScore(int getscore);

    sf::Vector2f getposition();
    sf::FloatRect getBounds() const; // ������ײ��

    void update(float deltaTime); // ����λ�á��ٶȵ�
    void draw(sf::RenderWindow& window); // ���ƽ�ɫ
    
    void takeDamage(int dmg);        // ��Ѫ
    bool isAlive() const;

private:
    sf::Sprite sprite;
    
    sf::Vector2f velocity;//��ҵ�ǰ���ƶ��ٶ�����
    sf::Vector2f position;

    float speed;
    float jumpForce;
    bool isOnGround;

    int health;
    int maxHealth;

    int score;

    void applyGravity(float deltaTime);
    void checkBounds(); // �����ڴ����ڻ��ͼ��
};

#endif // PLAYER_H
