#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class TileMap {
private:
    std::vector<std::vector<int>> tileData;
    sf::Texture tileSheet;
    int tileSize;

public:
    // 从文件加载瓦片数据
    void load(const std::string& filename) {
        // 这里需要实现从文件读取瓦片数据到tileData的逻辑
        // 例如从Tiled地图编辑器导出的文件格式解析
        // 暂未实现具体文件读取逻辑
    }

    // 渲染瓦片网格
    void draw(sf::RenderWindow& window) {
        for (size_t y = 0; y < tileData.size(); ++y) {
            for (size_t x = 0; x < tileData[y].size(); ++x) {
                int tileIndex = tileData[y][x];
                if (tileIndex != -1) {  // 假设-1表示空瓦片
                    sf::Sprite tileSprite(tileSheet);
                    tileSprite.setTexture(tileSheet);
                    // 根据tileIndex计算纹理矩形，假设瓦片集按顺序排列
                    int tileColumn = tileIndex % (tileSheet.getSize().x / tileSize);
                    int tileRow = tileIndex / (tileSheet.getSize().x / tileSize);
                    sf::IntRect textureRect = sf::Rect( 
                        sf::Vector2i(tileColumn * tileSize, tileRow * tileSize),
                        sf::Vector2i(tileSize, tileSize)
                    );
                    tileSprite.setTextureRect(textureRect);
                    tileSprite.setPosition({ x * tileSize, y * tileSize });
                    window.draw(tileSprite);
                }
            }
        }
    }

    // 判断特定瓦片是否为实心（可碰撞）
    bool isSolid(int tileX, int tileY) const {
        if (tileX < 0 || tileX >= static_cast<int>(tileData[0].size())
            || tileY < 0 || tileY >= static_cast<int>(tileData.size())) {
            return false;
        }
        int tileIndex = tileData[tileY][tileX];
        // 这里假设特定的tileIndex值表示实心瓦片，需根据实际瓦片数据定义
        return tileIndex == 1;  // 示例判断，实际应按需求修改
    }
};
