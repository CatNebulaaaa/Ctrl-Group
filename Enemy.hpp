#include <memory>
#include "Kirby.hpp"
#include "Entity.hpp"
#include "EnemyState.hpp"


//定义所有敌人的通用属性和行为
class Enemy : public Entity
{
protected:
    sf::Texture enemyTexture; // 敌人的纹理
    sf::IntRect currentTextureRect; // 当前动画帧的纹理矩形 (如果你有动画)
    int health = 100;             // 敌人生命值
    float moveSpeed = 80.f;       // 敌人巡逻速度
    float attackDamage = 10;      // 敌人每次接触攻击造成的伤害

    // 巡逻点
    sf::Vector2f patrolPointA;
    sf::Vector2f patrolPointB;
    sf::Vector2f currentTargetPoint; // 当前要前往的巡逻点
    bool movingToB = true;           // 标记当前是否正在从 A 走向 B

    // 用于攻击冷却（避免每帧都造成伤害）
    sf::Clock attackCooldownTimer;
    float attackCooldown = 1.0f; // 1秒内只能攻击一次

    // 指向 Kirby 实例的指针，以便敌人能找到并攻击 Kirby
    // 注意：这里使用裸指针简化，实际游戏中推荐使用更安全的 shared_ptr 或引用
    Kirby* kirbyPtr = nullptr;
public:
    // 构造函数：需要敌人的出生位置，以及两个巡逻点的位置,纹理，攻击力
    Enemy(sf::Vector2f spawnPos, sf::Vector2f pA, sf::Vector2f pB, const sf::Texture& texture,int amount);

    // 设置 Kirby 实例的指针，方便敌人与其交互
    void setKirbyTarget(Kirby* kirby);

    // 基类方法重写
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    // 敌人特有操作
    void takeDamage(int amount); // 受到伤害
    void patrol(float deltaTime); // 巡逻行为
    void checkAndAttackKirby();   // 检查并攻击 Kirby

    // 获取生命值 (可选，用于调试或UI显示)
    int getHealth() const { return health; }
};