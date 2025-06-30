#include "GameLevel.h"
#include <iostream>
#include "Game.h"

GameLevel::GameLevel() : currentState(LevelState::None) {
    if (!font.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF")) {
        std::cerr << "无法加载字体" << std::endl;
    }
    
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

void GameLevel::run(sf::RenderWindow& window) {
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
    if (!levelTexture.loadFromFile("image/level.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
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

    //创建玩家
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("image/kirby/stop.png")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // 加载失败
    }

    Player player(playerTexture);

    // 创建 Game 对象
    Game game(player);

    // 创建一个初始游戏状态（例如第一关）
    PlayState initialState;
    initialState.level = 1;
    initialState.playerX = 100;
    initialState.playerY = 300;
    // 加载关卡
    game.loadLevel(initialState, player);

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                currentState = LevelState::None;
                window.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                currentState = LevelState::None;
                draw(window);
                return;
            }
        }

        // 更新
        float deltaTime = clock.restart().asSeconds();
        game.update(player);

        // 渲染
        window.clear(sf::Color::Black);
        game.draw(window, player);
        window.display();
    }
}

void GameLevel::startLevel2(sf::RenderWindow& window) {
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                currentState = LevelState::None;
                window.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                currentState = LevelState::None;
                draw(window);
                return;
            }
        }
        sf::Text text(font);
        text.setString("Level 2 started");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Green);
        text.setPosition({ 100, 100 });
        window.clear();
        window.draw(text);
        window.display();
    }
}
    
void GameLevel::startLevel3(sf::RenderWindow& window) {
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                currentState = LevelState::None;
                window.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                currentState = LevelState::None;
                draw(window);
                return;
            }
        }
        sf::Text text(font);
        text.setString("Level 3 started");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Green);
        text.setPosition({ 100, 100 });
        window.clear();
        window.draw(text);
        window.display();
    }
}

void GameLevel::startEndless(sf::RenderWindow& window) {
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                currentState = LevelState::None;
                window.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                currentState = LevelState::None;
                draw(window);
                return;
            }
        }
        sf::Text text(font);
        text.setString("Endless started");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Green);
        text.setPosition({ 100, 100 });
        window.clear();
        window.draw(text);
        window.display();
    }
}
