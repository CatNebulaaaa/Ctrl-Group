#pragma once

#include "Entity.hpp"

class Kirby;

enum class BoostType {
	HEAL,  // 治疗：增加一定血量
	INVINCIBLE,  // 无敌
	HASTE,   //加速
	DAMAGEUP   //增加攻击力
};

class BoostItem : public Entity
{
protected:
	BoostType type = BoostType::HEAL;
	float size = 50.f;   //所占面积大小
	sf::RectangleShape shape; // Kirby 的图形形状:矩形；并用来设置Kirby的大小

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
	void setSpriteTextureRect(const sf::IntRect& rect) override;

};
