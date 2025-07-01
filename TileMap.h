#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    // 构造函数
    explicit TileMap(int tileSize = 32);

    // 加载地图资源
    bool load(const std::string& tilesetPath, const std::string& mapFile);

    // 碰撞检测
    bool isSolid(int tileX, int tileY, const std::vector<int>& solidTiles = { 1 }) const;

private:
    // 绘制接口
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // 构建顶点数组
    void buildVertexArray();

    // 成员变量
    sf::Texture m_tileset;
    sf::VertexArray m_vertices;
    std::vector<std::vector<int>> m_tileData;
    int m_tileSize;
};

#endif // TILE_MAP_HPP