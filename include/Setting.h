#pragma once
#include <SFML/Graphics.hpp>
#include "GameLevel.h"
#include "MainMenu.h"

enum class SeetingResult { Retry, Menu };
class Setting {
public:
    Setting(sf::RenderWindow& window);
    void run(sf::RenderWindow& window1, MainMenu& menu, sf::RenderWindow &window);

private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text retryText;
    sf::Text menuText;
    sf::RectangleShape retryBox;
    sf::RectangleShape menuBox;

    void handleEvents(sf::RenderWindow& window1, MainMenu& menu, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
};

