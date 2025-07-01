#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdint>

class CollisionManager {
public:
    // 基础矩形碰撞检测
    static bool checkRectCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        // 检查两个矩形是否有交集
        return rect1.findIntersection(rect2).has_value();
    }

    // 圆形与矩形碰撞检测
    static bool checkCircleRectCollision(
        const sf::Vector2f& circleCenter, // 圆形中心点
        float circleRadius,               // 圆形半径
        const sf::FloatRect& rect         // 矩形区域
    ) {
        // 计算圆形中心点距离矩形最近的点
        sf::Vector2f closestPoint(
            std::clamp(circleCenter.x, rect.position.x, rect.position.x + rect.size.x), // x轴方向上最近的点
            std::clamp(circleCenter.y, rect.position.y, rect.position.y + rect.size.y)  // y轴方向上最近的点
        );

        // 计算圆形中心点与最近点的距离
        float distance = std::hypot(
            circleCenter.x - closestPoint.x, // x轴方向上的距离差
            circleCenter.y - closestPoint.y  // y轴方向上的距离差
        );
        // 判断圆形是否与矩形发生碰撞
        return distance < circleRadius;
    }

    // 实体与瓦片地图碰撞检测
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
    // 像素级精确碰撞检测
    static bool checkPixelPerfectCollision(
        const sf::Sprite& sprite1,
        const sf::Sprite& sprite2,
        std::uint8_t alphaThreshold = 128
    ) {
        //获取全局边界并快速检查
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();   //获取sprite1的全局边界
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();   //获取sprite2的全局边界
        if (!bounds1.findIntersection(bounds2).has_value()) return false;  //无交集，即无碰撞

        //计算相交区域
        auto overlap = bounds1.findIntersection(bounds2).value();  //获取两个边界的交集

        //获取纹理数据
        const sf::Texture* tex1 = &sprite1.getTexture();    //获取sprite1的纹理指针
        const sf::Texture* tex2 = &sprite2.getTexture();    //获取sprite2的纹理指针
        //检查纹理指针是否为空，如果为空，说明没有纹理，返回false
        if (!tex1 || !tex2) return false;

        //交换为纹理坐标
        //将相交区域的左上角坐标转换为sprite1纹理坐标
        sf::Vector2f texPos1 = sprite1.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });
        //将相交区域的左上角坐标转换为sprite2纹理坐标
        sf::Vector2f texPos2 = sprite2.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });

        //遍历相交区域的每个像素(检查)
        for (int y = 0; y < overlap.size.y; ++y) {
            for (int x = 0; x < overlap.size.x; ++x) {
                sf::Vector2f texCoord1 = texPos1 + sf::Vector2f(x, y);    //将相交区域的每个像素转换为sprite1纹理坐标
                sf::Vector2f texCoord2 = texPos2 + sf::Vector2f(x, y);    //将相交区域的每个像素转换为sprite2纹理坐标

                //P.S.实际像素检查需要实现 getPixel() 功能
                //此处为伪代码（根据项目实际需求补充

                if (getPixelAlpha(tex1, texCoord1) > alphaThreshold &&
                    getPixelAlpha(tex2, texCoord2) > alphaThreshold) {
                    return true;  //都大于，即发生碰撞
                }
            }
        }
        return false;  //如果没有找到任何碰撞的像素
    }

private:
    // 辅助函数：获取纹理像素的Alpha值
    static std::uint8_t getPixelAlpha(const sf::Texture* texture, const sf::Vector2f& coord) {
        // 实际项目中需要将纹理复制到 sf::Image 或使用 GPU 着色器实现
        return 333 ;//(返回值只是演示)
    }
};