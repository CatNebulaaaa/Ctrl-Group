#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdint>

class CollisionManager {
public:
    // 基础矩形碰撞检测 (SFML 3.0 使用 intersects() 替代旧的 intersects 函数)
    static bool checkRectCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        return rect1.findIntersection(rect2).has_value();
    }
    // 圆形与矩形碰撞检测 (兼容 SFML 3.0 的数学工具)
    static bool checkCircleRectCollision(
        const sf::Vector2f& circleCenter,
        float circleRadius,
        const sf::FloatRect& rect
    ) {
        // SFML 3.0 推荐使用 std::hypot 计算距离
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

    // 实体与瓦片地图碰撞检测 (适配 SFML 3.0 的坐标系)
    static bool checkTileMapCollision(
        const sf::FloatRect& entityBounds,
        const std::vector<std::vector<int>>& tileData,
        int tileSize,
        const std::vector<int>& solidTiles = { 1 }
    ) {
        // 计算实体覆盖的瓦片范围
        int startX = static_cast<int>(entityBounds.position.x / tileSize);
        int endX = static_cast<int>((entityBounds.position.x + entityBounds.size.x) / tileSize);
        int startY = static_cast<int>(entityBounds.position.y / tileSize);
        int endY = static_cast<int>((entityBounds.position.y + entityBounds.size.y) / tileSize);

        // 检查每个可能碰撞的瓦片
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

    // 像素级精确碰撞检测 (适配 SFML 3.0 的纹理访问 API)
    static bool checkPixelPerfectCollision(
        const sf::Sprite& sprite1,
        const sf::Sprite& sprite2,
        std::uint8_t alphaThreshold = 128
    ) {
        // 获取全局边界并快速检查
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();
        if (!bounds1.findIntersection(bounds2).has_value()) return false;

        // 计算相交区域
        auto overlap = bounds1.findIntersection(bounds2).value();


        // 获取纹理数据 (SFML 3.0 使用更安全的纹理访问方式)
        const sf::Texture* tex1 = &sprite1.getTexture();
        const sf::Texture* tex2 = &sprite2.getTexture();
        if (!tex1 || !tex2) return false;

        // 转换为纹理坐标
        sf::Vector2f texPos1 = sprite1.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });
        sf::Vector2f texPos2 = sprite2.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });

        // 检查每个像素 (实际项目中建议使用优化算法)
        for (int y = 0; y < overlap.size.y; ++y) {
            for (int x = 0; x < overlap.size.x; ++x) {
                sf::Vector2f texCoord1 = texPos1 + sf::Vector2f(x, y);
                sf::Vector2f texCoord2 = texPos2 + sf::Vector2f(x, y);

                // 注意：实际像素检查需要实现 getPixel() 功能
                // 此处为伪代码，需要根据项目实际需求补充
                if (getPixelAlpha(tex1, texCoord1) > alphaThreshold &&
                    getPixelAlpha(tex2, texCoord2) > alphaThreshold) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    // 辅助函数：获取纹理像素的Alpha值 (需根据项目实现)
    static std::uint8_t getPixelAlpha(const sf::Texture* texture, const sf::Vector2f& coord) {
        // 实际项目中需要将纹理复制到 sf::Image 或使用 GPU 着色器实现
        // 此处返回默认值仅作演示
        return 255;
    }
};