#include "GameOverScene.h"
#include <iostream>

using namespace std;

GameOverScene::GameOverScene(sf::RenderWindow& window):gameOverText(font), retryText(font), menuText(font) {
    if (!font.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF"))
    {
        cerr << "字体文件无法打开" << endl;
    }

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(90);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color(255, 182, 193, 128)); // RGB = 浅粉，A = 半透明（0~255）
    gameOverText.setOutlineThickness(6.f);
    gameOverText.setPosition({ 800 / 2.f, 170.f });

    retryText.setFont(font);
    retryText.setString("Retry");
    retryText.setCharacterSize(60);
    retryText.setFillColor(sf::Color::White);
    retryText.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = 透明度（0-255），128 是半透明
    retryText.setOutlineThickness(4.f);
    retryText.setPosition({ 800 / 2, 250 });

    menuText.setFont(font);
    menuText.setString("Menu");
    menuText.setCharacterSize(60);
    menuText.setFillColor(sf::Color::White);
    menuText.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = 透明度（0-255），128 是半透明
    menuText.setOutlineThickness(4.f);   
    menuText.setPosition({ 800 / 2, 450 });

    sf::FloatRect yesBounds = retryText.getGlobalBounds();
    sf::FloatRect noBounds = menuText.getGlobalBounds();

    retryText.setOrigin({ yesBounds.size.x / 2.f + yesBounds.position.x, yesBounds.size.y / 2.f + yesBounds.position.y });
    menuText.setOrigin({ noBounds.size.x / 2.f + noBounds.position.x, noBounds.size.y / 2.f + noBounds.position.y });

    retryText.setPosition({ 180.f, 290.f });
    menuText.setPosition({ 470.f, 290.f });

    // 按钮的包围框（可选）
    sf::FloatRect yesLocalBounds = retryText.getLocalBounds();
    sf::FloatRect noLocalBounds = menuText.getLocalBounds();
    

    // 计算包围矩形大小时，只用 size，不用 position
    sf::Vector2f yesBoxSize(yesLocalBounds.size.x + 20.f, yesLocalBounds.size.y + 20.f);
    sf::Vector2f noBoxSize(noLocalBounds.size.x + 20.f, noLocalBounds.size.y + 20.f);

    retryBox.setSize(yesBoxSize);
    menuBox.setSize(noBoxSize);

    // origin设置为中心
    retryBox.setOrigin({ yesBoxSize.x / 2.f - 70.f, yesBoxSize.y / 2.f - 50.f });
    menuBox.setOrigin({ noBoxSize.x / 2.f - 70.f, noBoxSize.y / 2.f - 50.f });

    // 按钮矩形和文字同位置
    retryBox.setPosition(retryText.getPosition());
    menuBox.setPosition(menuText.getPosition());

    // 设置半透明背景颜色，避免完全遮挡文字
    retryBox.setFillColor(sf::Color(50, 50, 50, 150));  // 半透明暗灰色
    menuBox.setFillColor(sf::Color(50, 50, 50, 150));

}

void GameOverScene::run(sf::RenderWindow& window, GameLevel& level, MainMenu& menu) {
    while (window.isOpen()) {
        handleEvents(window,level, menu);
        render(window);
    }
}

void GameOverScene::handleEvents(sf::RenderWindow& window, GameLevel& level, MainMenu& menu) {
    while (const std::optional event = window.pollEvent())//看是否有事件
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            return;
        }
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                if (retryBox.getGlobalBounds().contains(mousePos)) {
                    level.run(window); // 开始闯关游戏
                }
                else if (menuBox.getGlobalBounds().contains(mousePos)) {
                    menu.handleInput(window);
                    return;
                }
            }
        }
    }
}



void GameOverScene::render(sf::RenderWindow& window) {
    sf::Texture gameoverTexture;//背景纹理
    if (!gameoverTexture.loadFromFile("image/win.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // 加载失败
    }
    sf::Sprite gameoverSprite(gameoverTexture);//背景精灵
    window.clear();
    window.draw(gameoverSprite);
    window.draw(gameOverText);
    window.draw(retryText);
    window.draw(menuText);
    window.draw(retryBox);
    window.draw(retryBox);
    window.display();
}
