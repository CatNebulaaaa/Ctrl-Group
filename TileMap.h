#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    // ���캯��
    explicit TileMap(int tileSize = 32);

    // ���ص�ͼ��Դ
    bool load(const std::string& tilesetPath, const std::string& mapFile);

    // ��ײ���
    bool isSolid(int tileX, int tileY, const std::vector<int>& solidTiles = { 1 }) const;

private:
    // ���ƽӿ�
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // ������������
    void buildVertexArray();

    // ��Ա����
    sf::Texture m_tileset;
    sf::VertexArray m_vertices;
    std::vector<std::vector<int>> m_tileData;
    int m_tileSize;
};

#endif // TILE_MAP_HPP