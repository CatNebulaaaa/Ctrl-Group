#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class TileMap {
private:
    std::vector<std::vector<int>> tileData;
    sf::Texture tileSheet;
    int tileSize;

public:
    // ���ļ�������Ƭ����
    void load(const std::string& filename) {
        // ������Ҫʵ�ִ��ļ���ȡ��Ƭ���ݵ�tileData���߼�
        // �����Tiled��ͼ�༭���������ļ���ʽ����
        // ��δʵ�־����ļ���ȡ�߼�
    }

    // ��Ⱦ��Ƭ����
    void draw(sf::RenderWindow& window) {
        for (size_t y = 0; y < tileData.size(); ++y) {
            for (size_t x = 0; x < tileData[y].size(); ++x) {
                int tileIndex = tileData[y][x];
                if (tileIndex != -1) {  // ����-1��ʾ����Ƭ
                    sf::Sprite tileSprite(tileSheet);
                    tileSprite.setTexture(tileSheet);
                    // ����tileIndex����������Σ�������Ƭ����˳������
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

    // �ж��ض���Ƭ�Ƿ�Ϊʵ�ģ�����ײ��
    bool isSolid(int tileX, int tileY) const {
        if (tileX < 0 || tileX >= static_cast<int>(tileData[0].size())
            || tileY < 0 || tileY >= static_cast<int>(tileData.size())) {
            return false;
        }
        int tileIndex = tileData[tileY][tileX];
        // ��������ض���tileIndexֵ��ʾʵ����Ƭ�������ʵ����Ƭ���ݶ���
        return tileIndex == 1;  // ʾ���жϣ�ʵ��Ӧ�������޸�
    }
};
