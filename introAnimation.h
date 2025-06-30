#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class IntroAnimation {
public:
    IntroAnimation(const std::string& folder, int frameCount, float frameDelay = 0.1f);
    bool play(sf::RenderWindow& window); // 播放完返回 true，表示可以进入游戏界面

private:
    std::vector<sf::Texture> frames;
    float delay;
    int totalFrames;
    std::string framePath;
};

