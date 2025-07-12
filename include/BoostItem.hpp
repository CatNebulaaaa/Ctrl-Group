#pragma once

#include "Entity.hpp"
#include <iostream>

class Kirby;

enum class BoostType {
	HEAL,  // 治疗：增加一定血量
	INVINCIBLE,  // 无敌
};

class BoostItem : public Entity
{
protected:
	BoostType type = BoostType::HEAL;
	float size = 10.f;   //所占面积大小

	//实现浮动
	sf::Clock floatClock;
	float floatAmplitude = 5.0f; // 浮动的最大幅度（上下总共 10 像素）
	float floatSpeed = 2.0f;    // 浮动的速度（越大浮动越快）
	float baseY;   //基准Y坐标

	float scaleFactor = 0.0125f;
public:
	BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp);

	BoostType getType() const { return type; }
	void boost(Kirby* kirby);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
	void handleCollision(Kirby& kirby);
	void setSpriteTextureRect(const sf::IntRect& rect) override;

	// 新增方法：获取纹理
	const sf::Texture& getTexture() const {
		// 确保纹理指针有效
		const sf::Texture* texturePtr = &sprite.getTexture();
		if (texturePtr) {
			return *texturePtr; // 解引用指针返回纹理引用
		}

	}

	// 新增方法：设置缩放
	void setScale(float scaleX, float scaleY) {
		sprite.setScale(sf::Vector2f(scaleX, scaleY));
		// 更新碰撞框
		boundingBox = sprite.getGlobalBounds();
	}

	// 新增方法：设置尺寸（统一尺寸）
	void setSize(float size) {
		this->size = size;
		// 根据纹理大小计算缩放
		const sf::Texture* texturePtr = &sprite.getTexture(); // 获取纹理指针
		if (texturePtr) {
			sf::Vector2u texSize = texturePtr->getSize();
			if (texSize.x > 0 && texSize.y > 0) {
				float scaleX = size / texSize.x;
				float scaleY = size / texSize.y;
				sprite.setScale({ scaleX, scaleY });
				boundingBox = sprite.getGlobalBounds();
			}
		}
	}
};
