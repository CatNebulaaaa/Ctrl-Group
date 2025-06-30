#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include<iostream>
// ����Kirby��Enemy���Զ������Ϸʵ����


class Kirby {
private:
    sf::Sprite sprite;  // ���ڱ�ʾKirby�ľ���
    sf::Texture texture;
    sf::FloatRect bounds;  // Kirby����ײ��
    float speed;  // Kirby���ƶ��ٶ�

public:
    // ���캯������ʼ��Kirby���������
    Kirby(): sprite(texture) { //ʹ�ó�Ա��ʼ���б�
        // �������һ��Ĭ�ϵ�Kirby����ͼƬ�����������ʵ������·���޸�
        if (!texture.loadFromFile("C:\\Users\\bxmf7\\Desktop\\Image\\Kirby.png")) {
            // �����������ʧ�ܵ�������������������Ϣ
            std::cerr << "Failed to load Kirby texture." << std::endl;
        }
        // ���ó�ʼλ��
        sprite.setPosition({ 100.f, 100.f });
        // ���ݾ���ߴ��ʼ����ײ��������Ծ���ߴ���Ϊ��ײ��ߴ�
        bounds = sprite.getGlobalBounds();
        speed = 100.f;  // �����ƶ��ٶ�
    }

    // ��ȡKirby�ľ��飬������Ⱦ
    sf::Sprite getSprite() const {
        return sprite;
    }

    // ��ȡKirby����ײ��
    sf::FloatRect getBounds() const {
        return bounds;
    }

    // �ƶ�Kirby
    void move(float dx, float dy) {
        sprite.move({ dx * speed, dy * speed });
        bounds.position += {dx * speed, dy* speed};
    }

    // ����Kirby��λ��
    void setPosition(float x, float y) {
        sprite.setPosition({ x, y });
        bounds.position.x = x;
        bounds.position.y = y;
    }
};
class Enemy {
public:
    sf::FloatRect getBounds() const {
        // ����Ӧ���ص�����ײ���ʵ���߼�
        return sf::FloatRect();
    }
};

class CollisionManager {
public:
    // �����ײ
    void checkCollisions(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies) {
        sf::FloatRect kirbyBounds = kirby.getBounds();
        for (auto& enemy : enemies) {
            sf::FloatRect enemyBounds = enemy->getBounds();
            if (kirbyBounds.findIntersection(enemyBounds)) {
                // ���������Ӧ����ײ�����߼�����δʵ�־���handleCollision����
                // ���Ը���ʵ�����������ײ�����Ӧ�����Ѫ��������
            }
        }
    }
};
