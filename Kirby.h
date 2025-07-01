#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.h" // ��ͼ�࣬������ͼ���ݺ͸�����Ϣ

class Kirby {
public:
    Kirby(); // ���캯������ʼ����ɫ����

    void setMap(TileMap* map);           // �󶨵�ͼָ�룬���ڲ�ѯ��ͼ��������
    void setPosition(float x, float y);  // ���ý�ɫ�ĳ�ʼλ��
    void handleInput();                  // ����������룬�ı��ɫ�ٶ�
    void update(float dt);               // ÿ֡���½�ɫ״̬��dtΪʱ����
    void draw(sf::RenderWindow& window); // ���ƽ�ɫ������
    sf::Vector2f getPosition();          // ��ȡ��ɫ��ǰλ��

    bool isDead() const;                 // ��ѯ��ɫ�Ƿ�����

    // �������жϽ�ɫ�ŵ��Ƿ���ڵ��涥��
    bool isOnGround();
    //�Զ��䵽������
    void fixInitialPosition();

private:
    sf::RectangleShape body;             // ��ɫ��ͼ�α�ʾ���þ���ģ����ײ��
    sf::Vector2f velocity = { 0.f,0.f };               // ��ɫ��ǰ�ٶȣ�ˮƽ�ʹ�ֱ��
    bool onGround=1;                      // ��ɫ�Ƿ�վ�ڵ��棨�ŵײ��ŵر�
    bool climbing=0;                      // ��ɫ�Ƿ���������״̬
    bool dead=0;                         // ��ɫ�Ƿ�������״̬

    TileMap* tileMap;                  // ָ���ͼʵ����ָ�룬�����ѯ��������

    void applyGravity(float dt);        // ����ɫӦ��������Ӱ�촹ֱ�ٶ�
    void checkCollisions();             // ��ײ��⣬�����ɫ���ͼ�Ľ���
    TileType getCurrentTileType();      // ��ȡ��ɫ�������ڸ��ӵ�����    
};
