#include "IntroAnimation.h"
#include <iostream>

IntroAnimation::IntroAnimation(const std::string& folder, int frameCount, float frameDelay)
    : delay(frameDelay), totalFrames(frameCount), framePath(folder)
{
    for (int i = 0; i < frameCount; ++i) {
        sf::Texture tex;
        std::string filename = folder + "/frame" + std::to_string(i) + ".png";
        if (!tex.loadFromFile(filename)) 
        {
            std::cerr << "无法加载帧图像: " << filename << std::endl;
        }
        else 
        {
            frames.push_back(tex);
        }
    }
    sf::Sprite sprite(frames[0]);
}

bool IntroAnimation::play(sf::RenderWindow& window)
{
    if (frames.empty()) return true;

    int currentFrame = 0;
    sf::Clock clock;  
    sf::Sprite sprite(frames[0]);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())//看是否有事件
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        // 获取窗口和纹理大小
        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = frames[0].getSize();

        // 计算缩放比例，保持宽高比
        float scaleX = static_cast<float>(winSize.x) / texSize.x;
        float scaleY = static_cast<float>(winSize.y) / texSize.y;

        sprite.setScale({ scaleX, scaleY }); // 应用等比缩放

        if (clock.getElapsedTime().asSeconds() > delay) {
            currentFrame++;
            if (currentFrame >= totalFrames) {
                return true;  // 动画播放完毕
            }
            
            sprite.setTexture(frames[currentFrame]);
            clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return false;
}
