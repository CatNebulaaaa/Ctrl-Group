#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <optional>

class CollisionManager {
public:
    // ����������ײ���
    static bool checkRectCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);

    // Բ���������ײ���
    static bool checkCircleRectCollision(
        const sf::Vector2f& circleCenter,
        float circleRadius,
        const sf::FloatRect& rect
    );

    // ʵ������Ƭ��ͼ��ײ���
    static bool checkTileMapCollision(
        const sf::FloatRect& entityBounds,
        const std::vector<std::vector<int>>& tileData,
        int tileSize,
        const std::vector<int>& solidTiles = { 1 }
    );

    // ���ؼ���ȷ��ײ���
    static bool checkPixelPerfectCollision(
        const sf::Sprite& sprite1,
        const sf::Sprite& sprite2,
        std::uint8_t alphaThreshold = 128
    );

private:
    // ������������ȡ�������ص�Alphaֵ
    static std::uint8_t getPixelAlpha(const sf::Texture* texture, const sf::Vector2f& coord);
};

#endif // COLLISION_MANAGER_HPP