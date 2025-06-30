#include "Enemy.hpp"
#include "Kirby.hpp"
#include <iostream> // ���ڵ������
#include <cmath>    // ���� std::sqrt, std::pow


// ���캯��
Enemy::Enemy(sf::Vector2f spawnPos, sf::Vector2f pA, sf::Vector2f pB, const sf::Texture& texture, int amount) :
    Entity(spawnPos.x, spawnPos.y, texture), // ���û��� Entity �Ĺ��캯��
    attackDamage(amount),
    patrolPointA(pA),
    patrolPointB(pB)
{
    // ��� ���� ��ͼ����״ (shape) ���ڶ�����̶���С����ײ�壺
    shape.setSize(sf::Vector2f(50.f, 50.f)); // ���磬���� ���� �Ĺ̶���СΪ 50x50 ����
    shape.setFillColor(sf::Color::White);  // Ĭ����ɫ

    // ȷ��Ѳ�ߵ�A��B�ǲ�ͬ�ģ�������ܻ�ԭ�ز���
    if (patrolPointA == patrolPointB) {
        // �����ͬ��Ĭ�������� A ����ұ�һ��Ѳ��
        patrolPointB.x += 100.f;
    }

    // ��ʼ����ǰĿ���Ϊ A �� B��ȡ�����ĸ���������ϣ������ʼ����
    // ��������Ĭ������ B ���ƶ�
    currentTargetPoint = patrolPointB;
    movingToB = true;


    // ���þ���ĳ�ʼ������Σ����������ļ�ֻ����һ������ͼ�񣬻��ߵ�һ����Ĭ��֡��
    // ������ж��������������ǵ�һ֡�ľ���
    currentTextureRect = sf::IntRect({ 0, 0 }, { 30, 30 }); // �����������ߴ��� 30x30
    sprite.setTextureRect(currentTextureRect);

    // ���þ���λ��
    sprite.setPosition(position);
    // ��ѡ�����þ���ԭ�㣬�����Ҫ���з�ת
    // sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}


// ���� Kirby ʵ����ָ��
void Enemy::setKirbyTarget(Kirby* kirby) {
    kirbyPtr = kirby;
}

// ���µ���״̬����Ϊ
void Enemy::update(float deltaTime) {
    if (!isAlive) {
        // ���������������ֹͣһ����Ϊ����ѡ�������������������մӳ����Ƴ�
        velocity = sf::Vector2f(0.f, 0.f);
        return;
    }

    patrol(deltaTime); // ִ��Ѳ����Ϊ

    // Ӧ���ٶȵ�λ��
    position += velocity * deltaTime;
    sprite.setPosition(position);
    boundingBox = sprite.getGlobalBounds(); // ������ײ�߽��

    // ��鲢���� Kirby
    checkAndAttackKirby();
}

// ���Ƶ���
void Enemy::draw(sf::RenderWindow& window) {
    if (isAlive) { // ֻ�л��ŵĵ��˲��ܱ�����
        window.draw(sprite);
    }
}

//�����������
void Enemy::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    // ÿ��������θı�����Kirby���Ӿ��ߴ���Ҫ���ֲ��䣬��Ҫ���µ������Ż�ԭ��
    // �������Ǽ���Kirby�Ĵ�С��shape������sprite��Ҫ��Ӧ��
    if (sprite.getLocalBounds().size.x > 0 && sprite.getLocalBounds().size.y > 0) {
        sprite.setScale({ shape.getSize().x / sprite.getLocalBounds().size.x,
            shape.getSize().y / sprite.getLocalBounds().size.y });
    }
    // ������ײ����ƥ���µľ���ߴ磨����������Ӱ���˳ߴ磩
    boundingBox = sprite.getGlobalBounds();
}

// �ܵ��˺�
void Enemy::takeDamage(int amount) {
    if (!isAlive) { // ���˾Ͳ�����������
        return;
    }

    health -= amount;
    std::cout << "�����ܵ� " << amount << " ���˺�����ǰ����ֵ: " << health << std::endl;

    if (health <= 0) {
        health = 0;
        isAlive = false; // ��ǵ���Ϊ����
        std::cout << "�����ѱ����ܣ�" << std::endl;
        // ���������������򴥷���������Ч����������ʧ��
        // sprite.setColor(sf::Color(255, 255, 255, 128)); // ���͸����ʾ����
    }
    else {
        // ��ѡ�������ܻ���������Ч
        sprite.setColor(sf::Color::Red); // ���ݱ���ʾ�ܻ�
        // ������ɫ
        // static sf::Clock hitFlashTimer;
        // hitFlashTimer.restart();
        // ������ update �м�������ʱ�����ָ���ɫ
    }
}

// Ѳ����Ϊ
void Enemy::patrol(float deltaTime) {
    sf::Vector2f target = currentTargetPoint;
    sf::Vector2f direction = target - position; // ���㷽������

    // �������Ŀ���ľ���
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f) { // �������Ŀ���ǳ��� (��ֵ 5.0f ���⸡�������)
        // ���ﵱǰĿ��㣬�л�Ŀ��
        if (movingToB) {
            currentTargetPoint = patrolPointA; // �л��� A ��
            movingToB = false;
        }
        else {
            currentTargetPoint = patrolPointB; // �л��� B ��
            movingToB = true;
        }
        // ȷ���ٶȹ��㣬�ȴ���һ֡���¼��㷽��
        velocity.x = 0;
        // ��ѡ��������ı���˳���
        // sprite.setScale(movingToB ? 1.f : -1.f, 1.f);
    }
    else {
        // ��Ŀ����ƶ�
        // ��һ�����������������ٶ�
        velocity.x = (direction.x / distance) * moveSpeed;
        // ����д�ֱ�����Ѳ�ߣ�ҲҪ���� velocity.y

        // �����ƶ�������¾��鳯�� (�������ԭ�������ģ�-1.f �ᷭת)
        if (velocity.x > 0) {
            sprite.setScale({ 1.f, 1.f }); // ����
        }
        else if (velocity.x < 0) {
            sprite.setScale({ -1.f, 1.f }); // ����
        }
    }
}

// ��鲢���� Kirby
void Enemy::checkAndAttackKirby() {
    if (!isAlive || kirbyPtr == nullptr || !kirbyPtr->getIsAlive()) {
        return; // ���˻� Kirby �������� Kirby ʵ��δ����
    }

    // ������˺� Kirby ����ײ�߽��ص�
    if (boundingBox.findIntersection(kirbyPtr->getGlobalBounds())) {
        // ��鹥����ȴʱ��
        if (attackCooldownTimer.getElapsedTime().asSeconds() >= attackCooldown) {
            std::cout << "���˽Ӵ��� Kirby������˺���" << std::endl;
            kirbyPtr->takeDamage(attackDamage); // Kirby �ܵ��˺� (Kirby ��� takeDamage() ����Ҫ����)
            attackCooldownTimer.restart(); // ���ù�����ȴ��ʱ��
        }
    }
}
