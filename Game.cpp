#include "Game.h"
#include <iostream>
#include "PlayState.h"

Game::Game(Player& player)//���õ���player������
    : map(), currentplaystate() {
    
}

void Game::loadLevel(PlayState& currentplaystate1, Player& player) {
    currentplaystate = currentplaystate1;

    // ʾ�������ص�ͼ
    switch (currentplaystate.level) {
    case 1:
        map.loadFromFile("image/assets/forest_map.txt");
        player.setposition(100, 300); // �������
        break;
    case 2:
        map.loadFromFile("image/assets/castle_map.txt");
        player.setposition(100, 300); // �������  
        break;
    case 3: 
        map.loadFromFile("image/assets/snow_map.txt");
        player.setposition(100, 250);
        break;
        /*
    case 4:
        map.loadFromFile("mapEndless.txt");
        player.setposition(100, 200);
        break;*/
    default: break;
    }
    
    // ���Լ������֡����ˡ����ߵ�����
    /*
    enemyManager.clear();
    enemyManager.spawnEnemiesForLevel(level);*/
}

void Game::savegamestate()
{
    saveGame(currentplaystate);
}
/*
//����main�����İ���֮��
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    PlayState playtruestate;
    playtruestate.level = currentLevel;
    playtruestate.playerX = player.getPosition().x;
    playtruestate.playerY = player.getPosition().y;
    playtruestate.health = player.getHealth();
    playtruestate.lives = player.getLives();
    playtruestate.score = player.getScore();
    game.savegamestate;//Ҫ��ǰ�洴��һ��Game��
    std::cout << "�浵�ɹ���\n";
}
*/

void Game::draw(sf::RenderWindow& window, Player& player)
{
    map.draw(window);
    //player.draw(window);
}

void Game::update(Player& player) {
    // 1. ����������루���緽�������Ծ�ȣ�
    player.handleInput();

    // 2. �������״̬�����ƶ��������ȣ�
    sf::Clock clock;
    float deltaTime = clock.restart().asSeconds();
    player.update(deltaTime); // ����е�ͼ��ײ���� map �ж�

    // 4. �ж��Ƿ�ͨ��
    if (isLevelFinished(player)) {
        currentplaystate.level += 1;
        std::cout << "��ϲͨ�أ�������һ�� Level " << currentplaystate.level << std::endl;
        loadLevel(currentplaystate, player);
    }

    // 5. ��ѡ���ж���Ϸ�Ƿ�ʧ��
    if (player.getposition().y > 1000) { // ��ҵ���ȥ��������
        std::cout << "�����ȥ�ˣ����¿�ʼ��ǰ�ؿ���" << std::endl;
        loadLevel(currentplaystate, player);
    }
}
