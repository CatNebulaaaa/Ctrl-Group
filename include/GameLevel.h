#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <SFML/Audio.hpp>

enum class LevelState {
    None,
    Level1,
    Level2,
    Level3,
    Endless
};

class GameLevel {
public:
    GameLevel();
    LevelState handleInput(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void startLevel1(sf::RenderWindow& window);
    void startLevel2(sf::RenderWindow& window);
    void startLevel3(sf::RenderWindow& window);
    void startEndless(sf::RenderWindow& window);
    void run(sf::RenderWindow& window); // 显示关卡选择界面

    void loadSounds();  //加载音频资源

private:
    sf::Font font;
    std::vector<sf::Text> levelTexts;
    std::vector<sf::RectangleShape> levelBoxes;
    LevelState currentState;

    sf::Music menuMusic;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
};
