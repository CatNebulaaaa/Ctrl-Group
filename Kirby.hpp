#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // 用于调试输出
#include <optional> // 用于 SFML 3.0.0 的事件处理

#include "Entity.hpp"
#include "BoostItem.hpp"

class Enemy;

//卡比：玩家控制的角色
//移动功能：行走，跳跃，下蹲；核心技能：吸入，吐出（星星/敌人）；生命值和其他状态；动画管理：待机，行走，跳跃，吸入等
class Kirby : public Entity
{
protected:
	sf::IntRect standingRect;
	sf::IntRect crouchingRect;
	sf::Texture kirbyTexture;
	sf::RectangleShape shape; // Kirby 的图形形状:矩形；并用来设置Kirby的大小

	int health = 100; //生命值
	int maxHealth = 100;
	bool isGrounded = false; //是否在地面
	bool jumpPressed = false;
	bool crouchPressed = false; //是否按下下蹲键（↓或S键）
	bool facingRight = true; //朝向

	//实现攻击
	sf::IntRect attackRect;
	float attackDamage = 10.f;

	bool isAttacking = false;      
	sf::Clock attackTimer;      // 攻击动画所用时间的计时器
	float attackDuration = 0.5f;       // 所用时间设置为0.5s
	sf::CircleShape attackRangeShape;
	float attackRadius = 50.f;      //攻击半径设置为50像素

	std::vector<std::shared_ptr<Enemy>>* enemiesPtr; // 敌人的指针列表

	//移动
	float moveSpeed = 120.f; //被设定的速度，和基类中的velocity（当前速度状态）不同
	float jumpSpeed = 250.f;

	//增益状态管理
	bool hasSpeedBoost = false;
	sf::Clock speedBoostTimer;
	float speedBoostDuration = 5.0f; // 加速持续时间

	bool hasAttackBoost = false;
	sf::Clock attackBoostTimer;
	float attackBoostDuration = 7.0f; // 增伤持续时间
	float currentAttackDamage = attackDamage;

	sf::Clock invincibilityTimer;
	bool isInvincible = false;
	float invincibilityDuration = 2.0f;  //无敌持续时间


public:
	//构造函数
	Kirby(const sf::Texture& texture, sf::Vector2f spawnPos); //spawn position:出生位置

	//基类方法重写
	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

	//Kirby独有操作
	void handleInput(const std::optional<sf::Event>& event); //处理单次输入事件（如按键，鼠标点击）
	void updateInput(); // 用于实时输入更新的方法
	void updateLogic(); //根据实时状态更新游戏逻辑
	void attack(std::vector<std::shared_ptr<Enemy>>* enemies); //攻击
	void takeDamage(int amount); //受到伤害

	// 获取卡比的攻击力（敌人或其他对象可能需要）
	float getAttackDamage() const { return currentAttackDamage; }
	// 获取生命值
	int getCurrentHealth() const { return health; }
	int getMaxHealth() const { return maxHealth; }

	//声明友元函数，便于增益果实产生作用
	friend void BoostItem::boost(Kirby* kirby);
};
