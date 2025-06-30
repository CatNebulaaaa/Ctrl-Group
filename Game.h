#include "Map.h"
#include "GameLevel.h"
#include "PlayState.h"
#include "Player.h"

class Game {
public:   
    Game(Player &player);
    void loadLevel(PlayState &currentplaystate1 , Player& player);//加载地图
    void update(Player& player);//更新玩家动作，位置等信息，以及是否成功/失败
    void draw(sf::RenderWindow& window, Player& player);//将地图和玩家画上去
    void savegamestate();//保存游戏进度
    

private:
    Map map;
    PlayState currentplaystate;
    
};
