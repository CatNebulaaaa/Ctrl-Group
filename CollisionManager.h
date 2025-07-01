#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <optional>

class CollisionManager {
public:
    // 基础矩形碰撞检测
    static bool checkRectCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);

    // 圆形与矩形碰撞检测
    static bool checkCircleRectCollision(
        const sf::Vector2f& circleCenter,
        float circleRadius,
        const sf::FloatRect& rect
    );

    // 实体与瓦片地图碰撞检测
    static bool checkTileMapCollision(
        const sf::FloatRect& entityBounds,
        const std::vector<std::vector<int>>& tileData,
        int tileSize,
        const std::vector<int>& solidTiles = { 1 }
    );

    // 像素级精确碰撞检测
    static bool checkPixelPerfectCollision(
        const sf::Sprite& sprite1,
        const sf::Sprite& sprite2,
        std::uint8_t alphaThreshold = 128
    );

private:
    // 辅助函数：获取纹理像素的Alpha值
    static std::uint8_t getPixelAlpha(const sf::Texture* texture, const sf::Vector2f& coord);
};

#endif // COLLISION_MANAGER_HPP