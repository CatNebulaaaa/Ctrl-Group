#include "GameOverScene.h"
#include <iostream>

using namespace std;

GameOverScene::GameOverScene(sf::RenderWindow& window):gameOverText(font), retryText(font), menuText(font) {
    if (!font.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF"))
    {
        cerr << "�����ļ��޷���" << endl;
    }

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(90);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color(255, 182, 193, 128)); // RGB = ǳ�ۣ�A = ��͸����0~255��
    gameOverText.setOutlineThickness(6.f);
    gameOverText.setPosition({ 800 / 2.f, 170.f });

    retryText.setFont(font);
    retryText.setString("Retry");
    retryText.setCharacterSize(60);
    retryText.setFillColor(sf::Color::White);
    retryText.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = ͸���ȣ�0-255����128 �ǰ�͸��
    retryText.setOutlineThickness(4.f);
    retryText.setPosition({ 800 / 2, 250 });

    menuText.setFont(font);
    menuText.setString("Menu");
    menuText.setCharacterSize(60);
    menuText.setFillColor(sf::Color::White);
    menuText.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = ͸���ȣ�0-255����128 �ǰ�͸��
    menuText.setOutlineThickness(4.f);   
    menuText.setPosition({ 800 / 2, 450 });

    sf::FloatRect yesBounds = retryText.getGlobalBounds();
    sf::FloatRect noBounds = menuText.getGlobalBounds();

    retryText.setOrigin({ yesBounds.size.x / 2.f + yesBounds.position.x, yesBounds.size.y / 2.f + yesBounds.position.y });
    menuText.setOrigin({ noBounds.size.x / 2.f + noBounds.position.x, noBounds.size.y / 2.f + noBounds.position.y });

    retryText.setPosition({ 180.f, 290.f });
    menuText.setPosition({ 470.f, 290.f });

    // ��ť�İ�Χ�򣨿�ѡ��
    sf::FloatRect yesLocalBounds = retryText.getLocalBounds();
    sf::FloatRect noLocalBounds = menuText.getLocalBounds();
    

    // �����Χ���δ�Сʱ��ֻ�� size������ position
    sf::Vector2f yesBoxSize(yesLocalBounds.size.x + 20.f, yesLocalBounds.size.y + 20.f);
    sf::Vector2f noBoxSize(noLocalBounds.size.x + 20.f, noLocalBounds.size.y + 20.f);

    retryBox.setSize(yesBoxSize);
    menuBox.setSize(noBoxSize);

    // origin����Ϊ����
    retryBox.setOrigin({ yesBoxSize.x / 2.f - 70.f, yesBoxSize.y / 2.f - 50.f });
    menuBox.setOrigin({ noBoxSize.x / 2.f - 70.f, noBoxSize.y / 2.f - 50.f });

    // ��ť���κ�����ͬλ��
    retryBox.setPosition(retryText.getPosition());
    menuBox.setPosition(menuText.getPosition());

    // ���ð�͸��������ɫ��������ȫ�ڵ�����
    retryBox.setFillColor(sf::Color(50, 50, 50, 150));  // ��͸������ɫ
    menuBox.setFillColor(sf::Color(50, 50, 50, 150));

}

void GameOverScene::run(sf::RenderWindow& window, GameLevel& level, MainMenu& menu) {
    while (window.isOpen()) {
        handleEvents(window,level, menu);
        render(window);
    }
}

void GameOverScene::handleEvents(sf::RenderWindow& window, GameLevel& level, MainMenu& menu) {
    while (const std::optional event = window.pollEvent())//���Ƿ����¼�
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
                    level.run(window); // ��ʼ������Ϸ
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
    sf::Texture gameoverTexture;//��������
    if (!gameoverTexture.loadFromFile("image/win.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // ����ʧ��
    }
    sf::Sprite gameoverSprite(gameoverTexture);//��������
    window.clear();
    window.draw(gameoverSprite);
    window.draw(gameOverText);
    window.draw(retryText);
    window.draw(menuText);
    window.draw(retryBox);
    window.draw(retryBox);
    window.display();
}
