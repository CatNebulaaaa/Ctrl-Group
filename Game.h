#pragma once
#include "TileMap.h"
#include <SFML/Graphics.hpp>  // һ��Ҫ�������
#include "GameLevel.h"
#include "PlayState.h"
#include "Kirby.h"

class Game {
public:
    Game(Kirby& player);
    void loadLevel(PlayState& currentplaystate1, Kirby& player);//���ص�ͼ
    void update(Kirby& player);//������Ҷ�����λ�õ���Ϣ���Լ��Ƿ�ɹ�/ʧ��
    void draw(sf::RenderWindow& window, Kirby& player);//����ͼ����һ���ȥ
    void savegamestate();//������Ϸ����
    TileMap& getTileMap();


private:
    TileMap map;
    sf::Texture tileset;    // ��Ƭ��������ͼ��
    PlayState currentplaystate;

};