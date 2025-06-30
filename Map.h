#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map {
public:
    Map();
    bool loadFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::vector<int>> tiles; // ��ͼ�ĸ������ݣ�ÿ�� int �� tile ID��,��ʾһ����ά���飬���ͼ�ĸ���
    sf::Texture tileset;                 // ����ש����ͼ
    sf::Sprite tileSprite;              // ���ڻ��Ƶ���ש��

    int tileSize = 32;                  // ÿ�����ӵ����ش�С��32x32��
};

