#ifndef ANIMATION_CONTROLLER_HPP
#define ANIMATION_CONTROLLER_HPP
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <vector>
#include <string>

class AnimationController {
public:
    // ���캯��
    AnimationController() = default;

    // ��������
    void setTexture(sf::Texture& texture);

    // ��Ӷ���
    void addAnimation(const std::string& name,
        const std::vector<sf::IntRect>& frames,
        sf::Time frameTime);

    // ���õ�ǰ����
    void setAnimation(const std::string& name);

    // ���¶���
    void update(sf::Time deltaTime);

    // Ӧ�õ�����
    void applyToSprite(sf::Sprite& sprite) const;

private:
    sf::Texture* m_texture = nullptr;
    std::map<std::string, std::vector<sf::IntRect>> m_animations;
    sf::Time m_frameDuration;
    sf::Time m_elapsedTime;
    size_t m_currentFrame = 0;
    bool m_isLooping = true;
    std::string currentAnimation;
};

#endif // ANIMATION_CONTROLLER_HPP