#include "Goal.hpp"

// ���캯��ʵ��
Goal::Goal(float x, float y, const sf::Texture& texture, const sf::IntRect& textureRect)
    : Entity(x, y, texture) // ���û��๹�캯��
{
    // ���þ�����������
    setSpriteTextureRect(textureRect);

    // ������ײ�߽�
    boundingBox = sprite.getGlobalBounds(); // Ŀ��ͨ��Ҳ��Ҫһ����ײ�߽�����������Ƿ񵽴�
    this->isAlive = true; // Ŀ��Ĭ���ǡ����š���
    this->velocity = sf::Vector2f(0.f, 0.f); // Ŀ��ͨ�����ƶ�
    std::cout << "Goal created at (" << position.x << ", " << position.y << "), size ("
        << boundingBox.size.x << ", " << boundingBox.size.y << ")" << std::endl;
}

// ���þ���������ε�ʵ��
void Goal::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f });
    sprite.setPosition(position); // ȷ������λ����ʵ��λ��ͬ��
}

// update ����ʵ��
// Ŀ��ͨ������Ҫ���£��������ж���������Ч��
void Goal::update(float deltaTime) {
    // ͨ��Ϊ��
    // ����ж�����animation->update(deltaTime);
}

// draw ����ʵ��
void Goal::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
