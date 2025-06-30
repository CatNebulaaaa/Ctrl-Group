#include "Map.h"
#include <fstream>
#include <sstream>
#include <iostream>

Map::Map(): tileSprite(tileset) {}

//���ص�ͼ���ݵĺ������� .txt �ļ��ж�ȡ������Ϣ��
bool Map::loadFromFile(const std::string& filename) {
    // ������ͼ��ͼ������������ tileset.png
    if (!tileset.loadFromFile("image/assets/mapall.png")) {
        std::cerr << "�޷����� tileset.png��" << std::endl;
        return false;
    }
    tileSprite.setTexture(tileset);

    // ��ȡ��ͼ�ı��ļ�
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "�޷��򿪵�ͼ�ļ���" << filename << std::endl;
        return false;
    }

    tiles.clear(); // ��վɵ�ͼ����
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        int tile;
        while (ss >> tile) {
            row.push_back(tile);
        }
        tiles.push_back(row);
    }

    std::cout << "��ͼ������ɣ�" << filename << std::endl;
    return true;
}

//����ͼ���Ƶ����� window ��
void Map::draw(sf::RenderWindow& window) {
    if (tileset.getSize().x == 0 || tileSize == 0)
        return; // ��ֹ����0����ͼδ����

    int tilesPerRow = tileset.getSize().x / tileSize; // ÿ���ж��ٸ�tile

    for (size_t y = 0; y < tiles.size(); ++y) {
        for (size_t x = 0; x < tiles[y].size(); ++x) {
            int tileID = tiles[y][x];
            if (tileID < 0) continue; // -1 ��ʾ�յ�

            // ������ͼ�е�����
            int tu = tileID % tilesPerRow; // tile �ڵڼ���
            int tv = tileID / tilesPerRow; // tile �ڵڼ���

            // �ü���ͼ���ж�Ӧ��ש��
            tileSprite.setTextureRect(sf::IntRect({ tu * tileSize, tv * tileSize }, { tileSize, tileSize }));

            // ���õ�ͼ�ϻ��Ƶ�λ��
            tileSprite.setPosition({ static_cast<float>(x * tileSize), static_cast<float>(y * tileSize) });

            // ������
            window.draw(tileSprite);
        }
    }
}
