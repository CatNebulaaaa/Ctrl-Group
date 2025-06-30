#pragma once

#include "Entity.hpp"

class Kirby;

enum class BoostType {
	HEAL,  // ���ƣ�����һ��Ѫ��
	INVINCIBLE,  // �޵�
	HASTE,   //����
	DAMAGEUP   //���ӹ�����
};

class BoostItem : public Entity
{
protected:
	BoostType type = BoostType::HEAL;
	float size = 50.f;   //��ռ�����С
	sf::RectangleShape shape; // Kirby ��ͼ����״:���Σ�����������Kirby�Ĵ�С

	//ʵ�ָ���
	sf::Clock floatClock;
	float floatAmplitude = 5.0f; // �����������ȣ������ܹ� 10 ���أ�
	float floatSpeed = 2.0f;    // �������ٶȣ�Խ�󸡶�Խ�죩
public:
	BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp);
	
	BoostType getType() const { return type; }
	void boost(Kirby* kirby);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
	void handleCollision(Entity& other) override; 
	void setSpriteTextureRect(const sf::IntRect& rect) override;

};
