#include "../include/GameLevel.h"
#include <iostream>
#include "../include/Game.h"

GameLevel::GameLevel() : currentState(LevelState::None), clickSound(clickBuffer) {
    if (!font.openFromFile("assets/fonts/ELEPHNT.TTF")) {
        std::cerr << "无法加载字体" << std::endl;
    }
    loadSounds();  // 加载音效
    std::vector<std::string> labels = {
        "Level 1", "Level 2", "Level 3", "Endless Mode"
    };

    float startY = 130.f;
    float gap = 110.f;

    for (int i = 0; i < labels.size(); ++i) {
        // 设置文本
        sf::Text text(font);
        text.setString(labels[i]);
        text.setCharacterSize(55);
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2 });
        text.setPosition({ 400.f, startY + i * gap });
        levelTexts.push_back(text);

        // 设置按钮框
        sf::RectangleShape box;
        box.setSize({ 400.f, 70.f });
        box.setFillColor(sf::Color(100, 100, 100, 150));
        box.setOrigin({ 200.f, 35.f });
        box.setPosition({ 400.f, startY + i * gap });
        levelBoxes.push_back(box);
    }
}
void GameLevel::loadSounds()
{
    if (!clickBuffer.loadFromFile("assets/sound/click1.wav")) {
        std::cerr << "无法加载点击音效！" << std::endl;
    }
    else {
        clickSound.setBuffer(clickBuffer);
    }
}
void GameLevel::run(sf::RenderWindow& window) {
    if (!menuMusic.openFromFile("assets/sound/menu.wav")) {
        std::cerr << "Failed to load menu music.\n";
        return;
    }
    menuMusic.setLooping(true);
    menuMusic.setLoopPoints({ sf::milliseconds(0), sf::seconds(150) });
    menuMusic.setVolume(50);  // 可选：设置音量
    menuMusic.play();
    currentState = LevelState::None;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
            currentState = handleInput(window);
            switch (currentState) {
            case LevelState::None:
                window.clear(sf::Color::Black);
                draw(window);
                window.display(); break;
            case LevelState::Level1:   startLevel1(window); break;
            case LevelState::Level2:   startLevel2(window); break;
            case LevelState::Level3:   startLevel3(window); break;
            case LevelState::Endless:  startEndless(window); break;
            default: break;
            }
        
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                currentState = LevelState::None;                
                return;
            }
        }
    }
}

LevelState GameLevel::handleInput(sf::RenderWindow& window) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (size_t i = 0; i < levelBoxes.size(); ++i) {
                    if (levelBoxes[i].getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentState = static_cast<LevelState>(i + 1); // enum 从 Level1 = 1 开始
                        return currentState;
                    }
                }
            }
        }
    }
}

void GameLevel::draw(sf::RenderWindow& window) {
    sf::Texture levelTexture;//背景纹理
    if (!levelTexture.loadFromFile("assets/image/level.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // 加载失败
    }
    sf::Sprite levelSprite(levelTexture);//背景精灵
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = levelTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    levelSprite.setScale({ scaleX, scaleY });
    window.clear();
    window.draw(levelSprite);
    for (size_t i = 0; i < levelBoxes.size(); ++i) {
        window.draw(levelBoxes[i]);
        window.draw(levelTexts[i]);
    }
}

void GameLevel::startLevel1(sf::RenderWindow& window) {
    Kirby player;
    PlayState playstate;
    playstate.level = 1;  // 第1关

    Game game(player);
    game.loadLevel(playstate, player);//已经加载了player地图以及初始位置

    sf::Clock clock;  //只创建一次时钟
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
        }
        float deltaTime = clock.restart().asSeconds();  //这才是真正的帧间隔
        game.update(player, deltaTime,window);//其中有player的update（也有每帧时间的定义），也有判断是否出界
        game.draw(window, player);
    }
}

void GameLevel::startLevel2(sf::RenderWindow& window) {
    Kirby player;
    PlayState playstate;
    playstate.level = 2;  // 第2关

    Game game(player);
    game.loadLevel(playstate, player);//已经加载了player地图以及初始位置
   
    sf::Clock clock;  //只创建一次时钟
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
        }
        float deltaTime = clock.restart().asSeconds();  //这才是真正的帧间隔
        game.update(player, deltaTime,window);//其中有player的update（也有每帧时间的定义），也有判断是否出界
        game.draw(window, player);
    }
}
    
void GameLevel::startLevel3(sf::RenderWindow& window) {
    Kirby player;
    PlayState playstate;
    playstate.level = 3;  // 第3关

    Game game(player);
    game.loadLevel(playstate, player);//已经加载了player地图以及初始位置

    sf::Clock clock;  //只创建一次时钟
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
        }
        float deltaTime = clock.restart().asSeconds();  //这才是真正的帧间隔
        game.update(player, deltaTime,window);//其中有player的update（也有每帧时间的定义），也有判断是否出界
        game.draw(window, player);
    }
}

void GameLevel::startEndless(sf::RenderWindow& window) {
    Kirby player;
    PlayState playstate;
    playstate.level = 4;  // 第4关

    Game game(player);
    game.loadLevel(playstate, player);//已经加载了player地图以及初始位置

    sf::Clock clock;  //只创建一次时钟
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
        }
        float deltaTime = clock.restart().asSeconds();  //这才是真正的帧间隔
        game.update(player, deltaTime,window);//其中有player的update（也有每帧时间的定义），也有判断是否出界
        game.draw(window, player);
    }
    menuMusic.stop();
}
