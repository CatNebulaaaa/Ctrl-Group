#include "MainMenu.h"
#include "introAnimation.h"
#include "GameLevel.h"
#include "PlayState.h"
#include "Game.h"

MainMenu::MainMenu(const sf::Sprite& bg) :title(font1), startButton(font2), state(GameState::Menu), backgroundSprite(bg)
{
    if (!font1.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF"))
    {
        cerr << "字体文件无法打开" << endl;
    }

    //设置标题
    title.setFont(font1);
    title.setString("Kirby Adventure");
    title.setCharacterSize(90);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color(255, 182, 193, 128)); // RGB = 浅粉，A = 半透明（0~255）
    title.setOutlineThickness(6.f);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({ titleBounds.size.x / 2.f+ titleBounds.position.x, titleBounds.size.y / 2.f+ titleBounds.position.y });
    title.setPosition({ 800 / 2.f, 170.f }); // 设置中心位置

    // 新增按钮
    vector<string> buttonNames = { "Load Game",  "Exit", "Settings", "Help" };
    buttonActions = {GameState::LoadGame, GameState::Exit,GameState::Settings, GameState::Help };

    float startY = 360.f;
    float gap = 70.f;

    for (int i = 0; i < buttonNames.size(); i++)
    {
        if (!font2.openFromFile("Roboto_Condensed-Regular.ttf"))
        {
            cerr << "字体文件无法打开" << endl;
        }
        sf::Text btn(font2);
        btn.setString(buttonNames[i]);
        btn.setCharacterSize(35);
        btn.setFillColor(sf::Color::White);
        btn.setOutlineColor(sf::Color(0, 0, 0, 128));
        btn.setOutlineThickness(4.f);

        sf::FloatRect textBounds = btn.getLocalBounds();
        btn.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
        // 设置统一位置
        sf::Vector2f position = { 710.f, startY + i * gap };
        btn.setPosition(position);
        buttons.push_back(btn);

        sf::RectangleShape box;
        sf::Vector2f boxSize(160.f, 50.f);
        box.setSize(boxSize);
        box.setFillColor(sf::Color(100, 100, 100, 150));
        box.setOrigin({ boxSize.x / 2.f , boxSize.y/2});
        box.setPosition(position);
        buttonBoxes.push_back(box);
    }
    // 设置开始按钮
    startButton.setFont(font2);
    startButton.setString("Start Game");
    startButton.setCharacterSize(90);
    startButton.setFillColor(sf::Color::White);
    startButton.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = 透明度（0-255），128 是半透明
    startButton.setOutlineThickness(4.f);
    startButton.setPosition({ 800 / 2, 350 }); // 设置中心

    // --- 中心对齐核心代码开始 ---
    sf::Vector2f centerPosition(800 / 2.f, 350.f);  // 画面中间，纵坐标固定 300
    sf::FloatRect textBounds = startButton.getLocalBounds();//得到开始按钮的大小
    startButton.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
    startButton.setPosition(centerPosition);//把按钮的中心放在窗口的（400, 300）这个位置上,前面并没有锚定在窗口的什么位置
    // 按钮的背景颜色
    // 设置按钮背景框（比文字大一点）
    sf::Vector2f padding(10.f, 10.f);  // 每边多出 10 像素
    sf::Vector2f boxSize(textBounds.size.x + 2 * padding.x, textBounds.size.y + 2 * padding.y);
    buttonBox.setSize(boxSize);
    buttonBox.setFillColor(sf::Color(100, 100, 100, 150));  // 半透明灰色
    buttonBox.setOrigin({ boxSize.x / 2.f, boxSize.y / 2.f });  // 也设置为中心对齐
    buttonBox.setPosition(centerPosition);
}

void MainMenu::showGameplay(sf::RenderWindow& window)
{
    IntroAnimation intro("image/animation", 23); // 30是帧数
    intro.play(window);
    GameLevel level;
    level.run(window); // 开始闯关游戏

    // 模拟按任意键返回菜单
    while (const std::optional event = window.pollEvent())//看是否有事件
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            return;
        }
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            state = GameState::Menu;
            handleInput(window);
        }
    }
}


GameState MainMenu::getState()
{
    return state;
}


void MainMenu::showLoadGame(sf::RenderWindow& window)
{
    sf::Font font1;
    if (!font1.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF")) {
        std::cerr << "无法加载字体\n";
        return;
    }
    
    // 创建提示文本
    sf::Text prompt(font1);
    prompt.setString("Continue game");
    prompt.setCharacterSize(80);
    prompt.setFillColor(sf::Color::White);
    prompt.setOutlineColor(sf::Color(255, 182, 193, 128)); // RGB = 浅粉，A = 半透明（0~255）
    sf::FloatRect promptBounds = prompt.getLocalBounds();
    prompt.setOutlineThickness(4.f);
    prompt.setOrigin({ promptBounds.size.x / 2.f + promptBounds.position.x, promptBounds.size.y / 2.f + promptBounds.position.y });
    prompt.setPosition({ 150.f, 150.f });
    
    // 创建“是”和“否”按钮
    sf::Text yesBtn(font1), noBtn(font1);
    yesBtn.setString("Yes");
    noBtn.setString("No");

    yesBtn.setCharacterSize(80);
    noBtn.setCharacterSize(80);

    yesBtn.setFillColor(sf::Color::Green);
    noBtn.setFillColor(sf::Color::Red);

    sf::FloatRect yesBounds = yesBtn.getGlobalBounds();
    sf::FloatRect noBounds = noBtn.getGlobalBounds();

    prompt.setOrigin({ yesBounds.size.x / 2.f + yesBounds.position.x, yesBounds.size.y / 2.f + yesBounds.position.y });
    prompt.setOrigin({ noBounds.size.x / 2.f + noBounds.position.x, noBounds.size.y / 2.f + noBounds.position.y });

    yesBtn.setPosition({ 180.f, 290.f });
    noBtn.setPosition({ 470.f, 290.f });

    // 按钮的包围框（可选）
    sf::FloatRect yesLocalBounds = yesBtn.getLocalBounds();
    sf::FloatRect noLocalBounds = noBtn.getLocalBounds();
    sf::RectangleShape yesBox;
    sf::RectangleShape noBox;

    // 计算包围矩形大小时，只用 size，不用 position
    sf::Vector2f yesBoxSize(yesLocalBounds.size.x + 20.f, yesLocalBounds.size.y + 20.f);
    sf::Vector2f noBoxSize(noLocalBounds.size.x + 20.f, noLocalBounds.size.y + 20.f);

    yesBox.setSize(yesBoxSize);
    noBox.setSize(noBoxSize);

    // origin设置为中心
    yesBox.setOrigin({ yesBoxSize.x / 2.f-70.f, yesBoxSize.y / 2.f-50.f });
    noBox.setOrigin({ noBoxSize.x / 2.f-70.f, noBoxSize.y / 2.f -50.f});

    // 按钮矩形和文字同位置
    yesBox.setPosition(yesBtn.getPosition());
    noBox.setPosition(noBtn.getPosition());

    // 设置半透明背景颜色，避免完全遮挡文字
    yesBox.setFillColor(sf::Color(50, 50, 50, 150));  // 半透明暗灰色
    noBox.setFillColor(sf::Color(50, 50, 50, 150));
    

    sf::Texture loadTexture;//背景纹理
    if (!loadTexture.loadFromFile("image/load.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // 加载失败
    }

    sf::Sprite loadSprite(loadTexture);//背景精灵
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = loadTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    loadSprite.setScale({ scaleX, scaleY });
    loadSprite.setColor(sf::Color(255, 255, 255, 200)); // 128 是半透明，0 完全透明，255 完全不透明
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                state = GameState::Exit;
                window.close();
                return;
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                    if (yesBox.getGlobalBounds().contains(mousePos)) {
                        sf::Texture loadTexture;//背景纹理
                        if (!loadTexture.loadFromFile("image/kirby/stop.png")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
                        {
                            return; // 加载失败
                        }
                        while (window.isOpen())
                        {
                            window.clear();
                            sf::Sprite spriteTexture(loadTexture);
                            window.draw(spriteTexture);
                            window.display();
                            int back = 0;
                            while (const std::optional event = window.pollEvent())
                            {
                                if (event->is<sf::Event::Closed>())
                                {
                                    state = GameState::Exit;
                                    window.close();
                                    return;
                                }
                                if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                                {
                                    back = 1;
                                }
                            }
                            if (back)
                            {
                                break;
                            }
                        }
                        // 加载存档
                        /*PlayState playstate;
                        if (loadGame(playstate)) {
                            // 加载成功，设置游戏
                            Player player(loadTexture);
                            Game gamenow(player);
                            gamenow.loadLevel(playstate, player);  // 你写的加载某一关函数
                            player.setposition(playstate.playerX, playstate.playerY);
                            player.setHealth(playstate.health);
                            player.setScore(playstate.score);
                        }
                        else {
                            std::cout << "未找到存档文件！\n";
                        }*/
                    }
                    else if (noBox.getGlobalBounds().contains(mousePos)) {
                        state = GameState::Menu;
                        showMenu(window);
                        return;
                    }                   
                }                
            }
            else if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                state = GameState::Menu;
                showMenu(window);
                return;
            }
        }
        
        window.clear();
        window.draw(loadSprite);
        window.draw(prompt);
        window.draw(yesBtn);
        window.draw(noBtn);
        window.draw(yesBox);
        window.draw(noBox);
        window.display();
    }

}


void MainMenu::showSettings(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF"))
    {
        std::cerr << "无法加载字体\n";
        return;
    }

    sf::Text SettingText(font);
    SettingText.setString("Seting\nPress Enter to return to menu.");
    SettingText.setCharacterSize(50);
    SettingText.setFillColor(sf::Color::White);
    SettingText.setOutlineColor(sf::Color(250, 100, 100, 128));  // A = 透明度（0-255），128 是半透明
    SettingText.setOutlineThickness(4.f);
    SettingText.setPosition({ 0, 0 });

    sf::Texture SettingTexture;//背景纹理
    if (!SettingTexture.loadFromFile("image/setting.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // 加载失败
    }
    sf::Sprite SettingSprite(SettingTexture);//背景精灵
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = SettingTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    SettingSprite.setScale({ scaleX, scaleY });
    SettingSprite.setColor(sf::Color(255, 255, 255, 200)); // 128 是半透明，0 完全透明，255 完全不透明

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                state = GameState::Exit;
                window.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                state = GameState::Menu;
                showMenu(window);
                return;
            }
        }

        window.clear();
        window.draw(SettingSprite);
        window.draw(SettingText);
        window.display();
    }
}
void MainMenu::showHelp(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.openFromFile("C:\\WINDOWS\\FONTS\\ELEPHNT.TTF")) 
    {
        std::cerr << "无法加载字体\n";
        return;
    }

    sf::Text helpText(font);
    helpText.setString("Help\nPress Enter to return to menu.");
    helpText.setCharacterSize(50);
    helpText.setFillColor(sf::Color::White);
    helpText.setOutlineColor(sf::Color(100, 50, 100, 138));  // A = 透明度（0-255），128 是半透明
    helpText.setOutlineThickness(4.f);
    helpText.setPosition({ 0, 0 });

    sf::Texture HelpTexture;//背景纹理
    if (!HelpTexture.loadFromFile("image/playbegin.jpg")) //可改为!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // 加载失败
    }
    sf::Sprite HelpSprite(HelpTexture);//背景精灵
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = HelpTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    HelpSprite.setScale({ scaleX, scaleY });
    HelpSprite.setColor(sf::Color(255, 255, 255, 200)); // 128 是半透明，0 完全透明，255 完全不透明

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                state = GameState::Exit;
                window.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                state = GameState::Menu;
                showMenu(window);
                return;
            }
        }

        window.clear();
        window.draw(HelpSprite);
        window.draw(helpText);
        window.display();
    }
}
void MainMenu::showMenu(sf::RenderWindow& window)
{
    while (window.isOpen() && state == GameState::Menu)
    {
        handleInput(window);  // 让 handleInput 负责事件响应（包括关闭）
    }
}

void MainMenu::handleInput(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            state = GameState::Exit;
            window.close();
            return;
        }
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                if (buttonBox.getGlobalBounds().contains(mousePos))
                {
                    state = GameState::Gameplay;
                    showGameplay(window);
                }
                for (size_t i = 0; i < buttonBoxes.size(); ++i)
                {
                     if (buttonBoxes[i].getGlobalBounds().contains(mousePos))
                     {
                         state = buttonActions[i];
                         if (state == GameState::LoadGame) showLoadGame(window);
                         if (state == GameState::Settings)showSettings(window);
                         if (state == GameState::Help)  showHelp(window);
                     }
                }
            }
        }
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            state = GameState::Menu;
            showMenu(window);
        }
    }
    draw(window);
}

void MainMenu::draw(sf::RenderWindow& window)
{
    // 绘制背景
     window.clear();
     window.draw(backgroundSprite);
     window.draw(title);
     window.draw(buttonBox);
     window.draw(startButton);
     for (auto& box : buttonBoxes)
        window.draw(box);
     for (auto& btn : buttons)
        window.draw(btn);
     window.display();
}