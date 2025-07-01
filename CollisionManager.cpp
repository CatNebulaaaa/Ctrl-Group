#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdint>

class CollisionManager {
public:
    // ����������ײ��� (SFML 3.0 ʹ�� intersects() ����ɵ� intersects ����)
    static bool checkRectCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        return rect1.findIntersection(rect2).has_value();
    }
    // Բ���������ײ��� (���� SFML 3.0 ����ѧ����)
    static bool checkCircleRectCollision(
        const sf::Vector2f& circleCenter,
        float circleRadius,
        const sf::FloatRect& rect
    ) {
        // SFML 3.0 �Ƽ�ʹ�� std::hypot �������
        sf::Vector2f closestPoint(
            std::clamp(circleCenter.x, rect.position.x, rect.position.x + rect.size.x),
            std::clamp(circleCenter.y, rect.position.y, rect.position.y + rect.size.y)
        );

        float distance = std::hypot(
            circleCenter.x - closestPoint.x,
            circleCenter.y - closestPoint.y
        );
        return distance < circleRadius;
    }

    // ʵ������Ƭ��ͼ��ײ��� (���� SFML 3.0 ������ϵ)
    static bool checkTileMapCollision(
        const sf::FloatRect& entityBounds,
        const std::vector<std::vector<int>>& tileData,
        int tileSize,
        const std::vector<int>& solidTiles = { 1 }
    ) {
        // ����ʵ�帲�ǵ���Ƭ��Χ
        int startX = static_cast<int>(entityBounds.position.x / tileSize);
        int endX = static_cast<int>((entityBounds.position.x + entityBounds.size.x) / tileSize);
        int startY = static_cast<int>(entityBounds.position.y / tileSize);
        int endY = static_cast<int>((entityBounds.position.y + entityBounds.size.y) / tileSize);

        // ���ÿ��������ײ����Ƭ
        for (int y = startY; y <= endY; ++y) {
            for (int x = startX; x <= endX; ++x) {
                if (y >= 0 && y < tileData.size() &&
                    x >= 0 && x < tileData[y].size())
                {
                    int tileIndex = tileData[y][x];
                    if (std::find(solidTiles.begin(), solidTiles.end(), tileIndex) != solidTiles.end()) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // ���ؼ���ȷ��ײ��� (���� SFML 3.0 ��������� API)
    static bool checkPixelPerfectCollision(
        const sf::Sprite& sprite1,
        const sf::Sprite& sprite2,
        std::uint8_t alphaThreshold = 128
    ) {
        // ��ȡȫ�ֱ߽粢���ټ��
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();
        if (!bounds1.findIntersection(bounds2).has_value()) return false;

        // �����ཻ����
        auto overlap = bounds1.findIntersection(bounds2).value();


        // ��ȡ�������� (SFML 3.0 ʹ�ø���ȫ��������ʷ�ʽ)
        const sf::Texture* tex1 = &sprite1.getTexture();
        const sf::Texture* tex2 = &sprite2.getTexture();
        if (!tex1 || !tex2) return false;

        // ת��Ϊ��������
        sf::Vector2f texPos1 = sprite1.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });
        sf::Vector2f texPos2 = sprite2.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });

        // ���ÿ������ (ʵ����Ŀ�н���ʹ���Ż��㷨)
        for (int y = 0; y < overlap.size.y; ++y) {
            for (int x = 0; x < overlap.size.x; ++x) {
                sf::Vector2f texCoord1 = texPos1 + sf::Vector2f(x, y);
                sf::Vector2f texCoord2 = texPos2 + sf::Vector2f(x, y);

                // ע�⣺ʵ�����ؼ����Ҫʵ�� getPixel() ����
                // �˴�Ϊα���룬��Ҫ������Ŀʵ�����󲹳�
                if (getPixelAlpha(tex1, texCoord1) > alphaThreshold &&
                    getPixelAlpha(tex2, texCoord2) > alphaThreshold) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    // ������������ȡ�������ص�Alphaֵ (�������Ŀʵ��)
    static std::uint8_t getPixelAlpha(const sf::Texture* texture, const sf::Vector2f& coord) {
        // ʵ����Ŀ����Ҫ�������Ƶ� sf::Image ��ʹ�� GPU ��ɫ��ʵ��
        // �˴�����Ĭ��ֵ������ʾ
        return 255;
    }
};