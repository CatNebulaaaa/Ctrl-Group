#pragma once
//该模块提供 RenderWindow、Text、RectangleShape 等类，用于渲染菜单和游戏。
#include <string>//<string>: 允许在菜单中使用 std::string 来显示文本（例如，“Kirby Adventure”）
#include <iostream>
#include <optional>
#include <vector>
#include "introAnimation.h"
#include <SFML/Audio.hpp>
#include "PlayState.h"
using namespace std;

enum class GameState { Menu, Gameplay, LoadGame, Settings, Help, Exit };//定义一个自定义类型 GameState 来追踪游戏的当前状态。
//列出了可能的值（Menu, Gameplay, Exit）。它用于控制游戏是否显示菜单、运行游戏玩法或退出。

class MainMenu
{
private:
    sf::Font font1;//存储用于标题的字体
    sf::Font font2;//存储用于开始游戏的字体
    sf::Text title;//显示游戏标题
    sf::Text startButton;//开始按钮
    sf::RectangleShape buttonBox;//开始按钮背景
    GameState state;//表示游戏状态
    sf::Sprite backgroundSprite;
    vector<sf::Text> buttons;             // 按钮文字
    vector<sf::RectangleShape> buttonBoxes; // 按钮背景框
    vector<GameState> buttonActions;      // 每个按钮对应的动

    sf::Music menuMusic;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;

public:
    MainMenu(const sf::Sprite& bg);
    void showGameplay(sf::RenderWindow& window);
    GameState getState();    
    void showLoadGame(sf::RenderWindow& window);
    void showSettings(sf::RenderWindow& window);
    void showHelp(sf::RenderWindow& window);
    void showMenu(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void loadSounds();  //加载音频资源
};