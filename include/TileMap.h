#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class TileType {
    empty,      // 空白：不可走
    ground,     // 地面：可行走
    water,      // 水：死亡
    ladder,     // 梯子：可上下爬
    obstacle,    // 障碍物：不可通行
    exit
};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool loadFromTMX(const std::string& filename, const sf::Texture& tileset, int tileWidth, int tileHeight);

    int getTileWidth() const;
    int getTileHeight() const;
    int getalltilex() const { return widthInTiles; }
    int getalltiley() const { return heightInTiles; }

    TileType getTileType(int row, int col) const;
    TileType getTileTypeAt(float x, float y) const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<std::vector<int>> mapData;
    std::vector<std::vector<TileType>> tileTypes;

    sf::VertexArray vertices;
    const sf::Texture* tilesetTexture;

    int tileWidth = 0;
    int tileHeight = 0;
    int widthInTiles = 0;
    int heightInTiles = 0;
};
