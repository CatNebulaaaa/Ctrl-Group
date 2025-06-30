#include "Map.h"
#include "GameLevel.h"
#include "PlayState.h"
#include "Player.h"

class Game {
public:   
    Game(Player &player);
    void loadLevel(PlayState &currentplaystate1 , Player& player);//���ص�ͼ
    void update(Player& player);//������Ҷ�����λ�õ���Ϣ���Լ��Ƿ�ɹ�/ʧ��
    void draw(sf::RenderWindow& window, Player& player);//����ͼ����һ���ȥ
    void savegamestate();//������Ϸ����
    

private:
    Map map;
    PlayState currentplaystate;
    
};
