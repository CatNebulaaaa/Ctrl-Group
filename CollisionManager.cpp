#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdint>

class CollisionManager {
public:
    // ����������ײ���
    static bool checkRectCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        // ������������Ƿ��н���
        return rect1.findIntersection(rect2).has_value();
    }

    // Բ���������ײ���
    static bool checkCircleRectCollision(
        const sf::Vector2f& circleCenter, // Բ�����ĵ�
        float circleRadius,               // Բ�ΰ뾶
        const sf::FloatRect& rect         // ��������
    ) {
        // ����Բ�����ĵ�����������ĵ�
        sf::Vector2f closestPoint(
            std::clamp(circleCenter.x, rect.position.x, rect.position.x + rect.size.x), // x�᷽��������ĵ�
            std::clamp(circleCenter.y, rect.position.y, rect.position.y + rect.size.y)  // y�᷽��������ĵ�
        );

        // ����Բ�����ĵ��������ľ���
        float distance = std::hypot(
            circleCenter.x - closestPoint.x, // x�᷽���ϵľ����
            circleCenter.y - closestPoint.y  // y�᷽���ϵľ����
        );
        // �ж�Բ���Ƿ�����η�����ײ
        return distance < circleRadius;
    }

    // ʵ������Ƭ��ͼ��ײ���
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
    // ���ؼ���ȷ��ײ���
    static bool checkPixelPerfectCollision(
        const sf::Sprite& sprite1,
        const sf::Sprite& sprite2,
        std::uint8_t alphaThreshold = 128
    ) {
        //��ȡȫ�ֱ߽粢���ټ��
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();   //��ȡsprite1��ȫ�ֱ߽�
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();   //��ȡsprite2��ȫ�ֱ߽�
        if (!bounds1.findIntersection(bounds2).has_value()) return false;  //�޽�����������ײ

        //�����ཻ����
        auto overlap = bounds1.findIntersection(bounds2).value();  //��ȡ�����߽�Ľ���

        //��ȡ��������
        const sf::Texture* tex1 = &sprite1.getTexture();    //��ȡsprite1������ָ��
        const sf::Texture* tex2 = &sprite2.getTexture();    //��ȡsprite2������ָ��
        //�������ָ���Ƿ�Ϊ�գ����Ϊ�գ�˵��û����������false
        if (!tex1 || !tex2) return false;

        //����Ϊ��������
        //���ཻ��������Ͻ�����ת��Ϊsprite1��������
        sf::Vector2f texPos1 = sprite1.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });
        //���ཻ��������Ͻ�����ת��Ϊsprite2��������
        sf::Vector2f texPos2 = sprite2.getInverseTransform().transformPoint({ overlap.position.x, overlap.position.y });

        //�����ཻ�����ÿ������(���)
        for (int y = 0; y < overlap.size.y; ++y) {
            for (int x = 0; x < overlap.size.x; ++x) {
                sf::Vector2f texCoord1 = texPos1 + sf::Vector2f(x, y);    //���ཻ�����ÿ������ת��Ϊsprite1��������
                sf::Vector2f texCoord2 = texPos2 + sf::Vector2f(x, y);    //���ཻ�����ÿ������ת��Ϊsprite2��������

                //P.S.ʵ�����ؼ����Ҫʵ�� getPixel() ����
                //�˴�Ϊα���루������Ŀʵ�����󲹳�

                if (getPixelAlpha(tex1, texCoord1) > alphaThreshold &&
                    getPixelAlpha(tex2, texCoord2) > alphaThreshold) {
                    return true;  //�����ڣ���������ײ
                }
            }
        }
        return false;  //���û���ҵ��κ���ײ������
    }

private:
    // ������������ȡ�������ص�Alphaֵ
    static std::uint8_t getPixelAlpha(const sf::Texture* texture, const sf::Vector2f& coord) {
        // ʵ����Ŀ����Ҫ�������Ƶ� sf::Image ��ʹ�� GPU ��ɫ��ʵ��
        return 333 ;//(����ֵֻ����ʾ)
    }
};