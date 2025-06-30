#include "MainMenu.h"

int main()
{
    //设置窗口
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Kirby Adventure");
    //加载窗口背景纹理
    sf::Texture backgroundTexture;//背景纹理
    if (!backgroundTexture.loadFromFile("image/background.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return -1; // 加载失败
    }
    sf::Sprite backgroundSprite(backgroundTexture);//背景精灵
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale({ scaleX, scaleY });
    //创建了一个菜单类
    MainMenu menu(backgroundSprite);

    while (window.isOpen())
    {
        menu.handleInput(window);
        
    }

    return 0;
}