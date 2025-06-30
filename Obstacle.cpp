#include "Obstacle.hpp"

// ���캯��ʵ��
Obstacle::Obstacle(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect, bool solid)
    : Entity(x, y, texture) // ���û��๹�캯��
{
    // ���þ�����������
    setSpriteTextureRect(textureRect);

    boundingBox = sprite.getGlobalBounds();
    this->isAlive = true; // �ϰ���ͨ��Ĭ���ǡ����š���
    this->velocity = sf::Vector2f(0.f, 0.f); // �ϰ���ͨ�����ƶ�
    std::cout << "Obstacle created at (" << position.x << ", " << position.y << "), size ("
        << boundingBox.size.x << ", " << boundingBox.size.y << ")" << std::endl;
}

// ���þ���������ε�ʵ��
void Obstacle::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f});
    sprite.setPosition(position); // ȷ������λ����ʵ��λ��ͬ��
}

// update ����ʵ��
// �ϰ���ͨ������Ҫ���£��������ж�����������Ϊ
void Obstacle::update(float deltaTime) {
    // �ϰ���ͨ���Ǿ�̬�ģ������������Ϊ��
    // ����ж�����animation->update(deltaTime);
}

// draw ����ʵ��
void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}