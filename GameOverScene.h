#pragma once
#include <SFML/Graphics.hpp>
#include "GameLevel.h"
#include "MainMenu.h"

class GameOverScene {
public:
    GameOverScene(sf::RenderWindow& window);
    void run(sf::RenderWindow& window, GameLevel& level, MainMenu &menu);

private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text retryText;
    sf::Text menuText;
    sf::RectangleShape retryBox;
    sf::RectangleShape menuBox;

    void handleEvents(sf::RenderWindow& window, GameLevel& level, MainMenu &menu);
    void render(sf::RenderWindow& window);
};
