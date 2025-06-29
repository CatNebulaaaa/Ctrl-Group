#include "Entity.hpp"
#include "BoostType.hpp"

class Kirby;

class BoostItem : public Entity
{
protected:
	BoostType type;
	float size = 50.f;   //所占面积大小

	//实现浮动
	sf::Clock floatClock;
	float floatAmplitude = 5.0f; // 浮动的最大幅度（上下总共 10 像素）
	float floatSpeed = 2.0f;    // 浮动的速度（越大浮动越快）
public:
	BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp);
	
	BoostType getType() const { return type; }
	void boost(Kirby* kirby);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
	void handleCollision(Entity& other) override; 

};