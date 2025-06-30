#include "Player.h"

Player::Player(sf::Texture& texture):sprite(texture){
    sprite.setScale({ 2.0f, 2.0f }); // �ɵ���
    score = 0;

    speed = 200.f;
    jumpForce = 500.f;
    isOnGround = false;

    health = 100;
    maxHealth = 100;
}

void Player::load(sf::Texture& texture, sf::RenderWindow& window) {
    if (!texture.loadFromFile("image/assets/kirby.png")) {
        // �滻Ϊ��Ľ�ɫ��ͼ
        throw std::runtime_error("�޷����ؽ�ɫ��ͼ��");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));  // ��ʼ֡
    window.draw(sprite);
}

void Player::setposition(int x, int y)
{
    position.x = x;
    position.y = y;
}

sf::Vector2f Player::getposition()
{
    return position;
}

void Player::handleInput() {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        velocity.x -= speed;
        sprite.setScale({- 2.0f, 2.0f}); // ����ת
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity.x += speed;
        sprite.setScale({ 2.0f, 2.0f }); // ����
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isOnGround) {
        velocity.y = -jumpForce;
        isOnGround = false;
    }
}

void Player::update(float deltaTime) {
    applyGravity(deltaTime);

    sprite.move(velocity * deltaTime);

    // �򻯴������Ӵ������������ y = 500��
    if (sprite.getPosition().y >= 500.f) {
        sprite.setPosition({ sprite.getPosition().x, 500.f });
        velocity.y = 0;
        isOnGround = true;
    }

    checkBounds();
}

void Player::applyGravity(float deltaTime) {
    float gravity = 981.f;
    velocity.y += gravity * deltaTime;
}

void Player::checkBounds() {
    // �ɼ�������λ�õ��߼�
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

bool Player::isAlive() const {
    return health > 0;
}


void Player::setposition(float x, float y)
{
    position = { x,y };
}
void Player::setHealth(int& gethealth)
{
    health = gethealth;
}
void Player::setScore(int getscore)
{
    score = getscore;
}