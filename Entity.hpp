#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // 用于调试输出
#include <optional> // 用于 SFML 3.0.0 的事件处理

//抽象基类Entity，所有动态对象的基本构建块
//定义物理属性：位置、速度；视觉属性：绘制、动画；交互：碰撞等
class Entity
{
protected:
	sf::Sprite sprite; //图像对象
	sf::Vector2f position;
	sf::Vector2f velocity; //速度（包含x和y方向）
	sf::FloatRect boundingBox; //碰撞检测
	//AnimationController* animation; //AnimationController类将在其他文件定义
	bool isAlive;
public:
	Entity(float x, float y, const sf::Texture& texture);
	virtual ~Entity() = default;     // 虚析构函数
	bool getIsAlive() const { return isAlive; }
	void setAlive(bool alive) { isAlive = alive; }
	virtual void setSpriteTextureRect(const sf::IntRect& rect) = 0; //设置精灵的纹理矩形
	virtual void update(float deltaTime) = 0; //deltaTime指时间帧
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual sf::FloatRect getGlobalBounds() const; //确认碰撞边界
	virtual void handleCollision(Entity& other) {}; //碰撞处理在CollisionManager中可能会调用
};
