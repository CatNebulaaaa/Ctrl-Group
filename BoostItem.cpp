#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // ���ڵ������
#include <optional> // ���� SFML 3.0.0 ���¼�����
#include <algorithm>
#include <cmath>

#include "BoostItem.hpp"
#include "Kirby.hpp"

class Kirby;

BoostItem::BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp) :
	Entity(spawnPos.x, spawnPos.y, texture),
	type(type){ }

//���û���Ե��ͻ��ƹ�ʵ
void BoostItem::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(sprite);
    }
}

//���һ�����¸����Ķ���
void BoostItem::update(float deltaTime) {
    // ��ȡ�Լ�ʱ��������������ʱ�䣨�룩
    float elapsedTime = floatClock.getElapsedTime().asSeconds();

    // ���� Y ��ĸ���ƫ����
    // std::sin �������ܻ��ȣ�floatSpeed ���Կ��Ƹ���Ƶ��
    float yOffset = std::sin(elapsedTime * floatSpeed) * floatAmplitude;

    // ������ƫ����Ӧ�õ������ Y ������
    // ע�⣺`position.x` ���ֲ��䣬`position.y` ���ϻ��� `position` �� Y �����ƫ����
    sprite.setPosition({ position.x, position.y + yOffset });

    // ������ײ���λ�ã���ƥ�両����ľ���λ��
    boundingBox = sprite.getGlobalBounds();
}

void BoostItem::handleCollision(Entity& other) {
    // 1. ȷ�� BoostItem ���ڻ״̬
    if (!isAlive) {
        return;
    }

    // 2. ���Խ� other ת��Ϊ Kirby ����
    // static_cast Ҳ���ԣ��� dynamic_cast ����ȫ����� other ���� Kirby�����᷵�� nullptr
    Kirby* kirbyPtr = dynamic_cast<Kirby*>(&other);

    // 3. ��� other ȷʵ�� Kirby ���� Kirby Ҳ����
    if (kirbyPtr && kirbyPtr->getIsAlive()) {
        // ��ȡ BoostItem �� Kirby ����ײ�߽��
        sf::FloatRect boostItemBounds = getGlobalBounds();
        sf::FloatRect kirbyBounds = kirbyPtr->getGlobalBounds();

        // ����Ƿ����κ��ص�
        if (const std::optional intersection = boundingBox.findIntersection(kirbyPtr->getGlobalBounds())) {
            float intersectionArea = intersection->size.x * intersection->size.y;
            // �ж��ص�����Ƿ񳬹� BoostItem ���������һ��
            if (intersectionArea >= (size * 0.5f)) {
                // ���������������������Ч��
                boost(kirbyPtr); // ������Ԫ���������� Kirby ����
            }
        }
    }
}


void BoostItem::boost(Kirby* kirby)
{
    if (!isAlive) {
        return;
    }

    std::cout << "Kirby ���������ʵ: ";

    switch (type) { // ���� BoostItem ���������ж�
    case BoostType::HEAL:
        // ����Ч�������� Kirby �ĵ�ǰ����ֵ�����������������ֵ
        kirby->health = std::min(kirby->health + 25, kirby->maxHealth);
        std::cout << "���ƣ���ǰ����ֵ: " << kirby->health << std::endl;
        break;

    case BoostType::DAMAGEUP:
        // ����Ч�������� Kirby �Ĺ���������������ʱ��
        kirby->currentAttackDamage = kirby->attackDamage * 2.0f; // ����������
        kirby->hasAttackBoost = true;
        kirby->attackBoostTimer.restart(); // ����������������ʱ��
        std::cout << "������������" << std::endl;
        break;

    case BoostType::INVINCIBLE:
        // �޵�Ч�������� Kirby Ϊ�޵�״̬����������ʱ��
        kirby->isInvincible = true;
        kirby->invincibilityTimer.restart(); // �����޵м�ʱ��
        std::cout << "����޵�Ч����" << std::endl;
        break;

    case BoostType::HASTE:
        //����Ч��
        kirby->velocity.x *= 1.5f; // �ٶ����� 50%
        kirby->hasSpeedBoost = true;
        kirby->speedBoostTimer.restart();
        std::cout << "��ü���Ч����" << std::endl;
        break;
    }
    // �����ʵ�����Ե����󣬽�����Ϊ����Ծ���Ա����Ϸ�������Ƴ�
    isAlive = false;
}


