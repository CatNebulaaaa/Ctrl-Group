#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // ���ڵ������
#include <optional> // ���� SFML 3.0.0 ���¼�����

#include "Entity.hpp"
#include "BoostItem.hpp"

class Enemy;

//���ȣ���ҿ��ƵĽ�ɫ
//�ƶ����ܣ����ߣ���Ծ���¶ף����ļ��ܣ����룬�³�������/���ˣ�������ֵ������״̬�������������������ߣ���Ծ�������
class Kirby : public Entity
{
protected:
	sf::IntRect standingRect;
	sf::IntRect crouchingRect;
	sf::Texture kirbyTexture;
	sf::RectangleShape shape; // Kirby ��ͼ����״:���Σ�����������Kirby�Ĵ�С

	int health = 100; //����ֵ
	int maxHealth = 100;
	bool isGrounded = false; //�Ƿ��ڵ���
	bool jumpPressed = false;
	bool crouchPressed = false; //�Ƿ����¶׼�������S����
	bool facingRight = true; //����

	//ʵ�ֹ���
	sf::IntRect attackRect;
	float attackDamage = 10.f;

	bool isAttacking = false;      
	sf::Clock attackTimer;      // ������������ʱ��ļ�ʱ��
	float attackDuration = 0.5f;       // ����ʱ������Ϊ0.5s
	sf::CircleShape attackRangeShape;
	float attackRadius = 50.f;      //�����뾶����Ϊ50����

	std::vector<std::shared_ptr<Enemy>>* enemiesPtr; // ���˵�ָ���б�

	//�ƶ�
	float moveSpeed = 120.f; //���趨���ٶȣ��ͻ����е�velocity����ǰ�ٶ�״̬����ͬ
	float jumpSpeed = 250.f;

	//����״̬����
	bool hasSpeedBoost = false;
	sf::Clock speedBoostTimer;
	float speedBoostDuration = 5.0f; // ���ٳ���ʱ��

	bool hasAttackBoost = false;
	sf::Clock attackBoostTimer;
	float attackBoostDuration = 7.0f; // ���˳���ʱ��
	float currentAttackDamage = attackDamage;

	sf::Clock invincibilityTimer;
	bool isInvincible = false;
	float invincibilityDuration = 2.0f;  //�޵г���ʱ��


public:
	//���캯��
	Kirby(const sf::Texture& texture, sf::Vector2f spawnPos); //spawn position:����λ��

	//���෽����д
	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

	//Kirby���в���
	void handleInput(const std::optional<sf::Event>& event); //�����������¼����簴�����������
	void updateInput(); // ����ʵʱ������µķ���
	void updateLogic(); //����ʵʱ״̬������Ϸ�߼�
	void attack(std::vector<std::shared_ptr<Enemy>>* enemies); //����
	void takeDamage(int amount); //�ܵ��˺�

	// ��ȡ���ȵĹ����������˻��������������Ҫ��
	float getAttackDamage() const { return currentAttackDamage; }
	// ��ȡ����ֵ
	int getCurrentHealth() const { return health; }
	int getMaxHealth() const { return maxHealth; }

	//������Ԫ���������������ʵ��������
	friend void BoostItem::boost(Kirby* kirby);
};
