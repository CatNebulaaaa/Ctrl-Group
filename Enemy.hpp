#include <memory>
#include "Kirby.hpp"
#include "Entity.hpp"
#include "EnemyState.hpp"


//�������е��˵�ͨ�����Ժ���Ϊ
class Enemy : public Entity
{
protected:
    sf::Texture enemyTexture; // ���˵�����
    sf::IntRect currentTextureRect; // ��ǰ����֡��������� (������ж���)
    int health = 100;             // ��������ֵ
    float moveSpeed = 80.f;       // ����Ѳ���ٶ�
    float attackDamage = 10;      // ����ÿ�νӴ�������ɵ��˺�

    // Ѳ�ߵ�
    sf::Vector2f patrolPointA;
    sf::Vector2f patrolPointB;
    sf::Vector2f currentTargetPoint; // ��ǰҪǰ����Ѳ�ߵ�
    bool movingToB = true;           // ��ǵ�ǰ�Ƿ����ڴ� A ���� B

    // ���ڹ�����ȴ������ÿ֡������˺���
    sf::Clock attackCooldownTimer;
    float attackCooldown = 1.0f; // 1����ֻ�ܹ���һ��

    // ָ�� Kirby ʵ����ָ�룬�Ա�������ҵ������� Kirby
    // ע�⣺����ʹ����ָ��򻯣�ʵ����Ϸ���Ƽ�ʹ�ø���ȫ�� shared_ptr ������
    Kirby* kirbyPtr = nullptr;
public:
    // ���캯������Ҫ���˵ĳ���λ�ã��Լ�����Ѳ�ߵ��λ��,����������
    Enemy(sf::Vector2f spawnPos, sf::Vector2f pA, sf::Vector2f pB, const sf::Texture& texture,int amount);

    // ���� Kirby ʵ����ָ�룬����������佻��
    void setKirbyTarget(Kirby* kirby);

    // ���෽����д
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    // �������в���
    void takeDamage(int amount); // �ܵ��˺�
    void patrol(float deltaTime); // Ѳ����Ϊ
    void checkAndAttackKirby();   // ��鲢���� Kirby

    // ��ȡ����ֵ (��ѡ�����ڵ��Ի�UI��ʾ)
    int getHealth() const { return health; }
};