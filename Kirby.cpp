#include <optional>
#include <vector>
#include <memory>

#include "Kirby.hpp"

// �������� ���캯�� ��������
Kirby::Kirby(const sf::Texture& texture, sf::Vector2f spawnPos)
    : Entity(spawnPos.x, spawnPos.y,texture), // ���û��� Entity �Ĺ��캯������ʼ�� position, velocity, isAlive
    invincibilityDuration(2.0f),   // �����޵г���ʱ��
    standingRect({ 0, 0 }, { 50, 100 }),
    crouchingRect({ 50, 0 }, { 50, 50 }),
    attackRect({ 100, 0 }, { 80, 100 }),
    attackDamage(10.f),
    currentAttackDamage(attackDamage)
{
    // ��� Kirby ��ͼ����״ (shape) ���ڶ�����̶���С����ײ�壺
    shape.setSize(sf::Vector2f(50.f, 50.f)); // ���磬���� Kirby �Ĺ̶���СΪ 50x50 ����
    shape.setFillColor(sf::Color::White);  // Ĭ����ɫ

    // ���þ���ĳ�ʼ������κ�λ��
    sprite.setTextureRect(standingRect); // ��ʼ��ʾվ������֡
    sprite.setPosition(position);        // ������λ������Ϊʵ�壨���ࣩ��λ��

    // ��ʼ��������Χ��״�������ڵ���ʱ���ӻ���
    attackRangeShape.setRadius(attackRadius);
    // ���ù�����Χ��ԭ�㵽�����ģ��Ա��ڶ�λ
    attackRangeShape.setOrigin({ attackRadius, attackRadius });
    attackRangeShape.setFillColor(sf::Color(255, 0, 0, 50)); // ��͸����ɫ
    attackRangeShape.setOutlineColor(sf::Color::Red);       // ��ɫ�߿�
    attackRangeShape.setOutlineThickness(1.f);               // �߿��ϸ
}


//�������� ��д�����麯�� ��������


//��ʱ�����
// update��ÿ֡���¿��ȵ�״̬���߼�
void Kirby::update(float deltaTime) {
    // �����޵�״̬��ʱ���Ӿ�Ч��
    if (isInvincible) {
        if (invincibilityTimer.getElapsedTime().asSeconds() >= invincibilityDuration) {
            isInvincible = false;
            std::cout << "Kirby �����޵С�" << std::endl;
            sprite.setColor(sf::Color::White); // �ָ�������ɫ
        }
        else {
            // �޵�ʱʹ������˸Ч��
            if (static_cast<int>(invincibilityTimer.getElapsedTime().asMilliseconds() / 100) % 2 == 0) {
                sprite.setColor(sf::Color(255, 255, 255, 128)); // ��͸��
            }
            else {
                sprite.setColor(sf::Color::White); // ��͸��
            }
        }
    }

    // ����Ч����ʱ������
    if (hasSpeedBoost) {
        if (speedBoostTimer.getElapsedTime().asSeconds() >= speedBoostDuration) {
            hasSpeedBoost = false;
            velocity.x /= 1.5f; // �ٶȻָ�������ֵ
            std::cout << "����Ч��������" << std::endl;
        }
    }

    // ����������Ч����ʱ������
    if (hasAttackBoost) {
        if (attackBoostTimer.getElapsedTime().asSeconds() >= attackBoostDuration) {
            hasAttackBoost = false;
            currentAttackDamage = attackDamage; // �������ָ�������ֵ
            std::cout << "����������Ч��������" << std::endl;
        }
    }

    // �������루ʵʱ������⣩
    updateInput();
    // ������Ϸ�߼�����������͵�ǰ״̬��
    updateLogic();

    // Ӧ���ٶȵ�λ�ã�ʵ���ƶ�
    position += velocity * deltaTime;

    // �������λ��ͬ�������ȵ��߼�λ��
    sprite.setPosition(position);

    // ������ײ�߽��ʹ���뾫��ĵ�ǰ״̬ƥ��
    boundingBox = sprite.getGlobalBounds();
}

//����sprite
void Kirby::draw(sf::RenderWindow& window)
{
    // ���û���� draw ������������� sprite
    // ����ֱ�� window.draw(sprite);
    window.draw(sprite);
}

//�����������
void Kirby::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    // ÿ��������θı�����Kirby���Ӿ��ߴ���Ҫ���ֲ��䣬��Ҫ���µ������Ż�ԭ��
    // �������Ǽ���Kirby�Ĵ�С��shape������sprite��Ҫ��Ӧ��
    if (sprite.getLocalBounds().size.x > 0 && sprite.getLocalBounds().size.y > 0) {
        sprite.setScale({ shape.getSize().x / sprite.getLocalBounds().size.x,
            shape.getSize().y / sprite.getLocalBounds().size.y });
    }
    // ������ײ����ƥ���µľ���ߴ磨����������Ӱ���˳ߴ磩
    boundingBox = sprite.getGlobalBounds();
}




//�������� Kirby���еĲ��� ��������

//������/���ĵ�������
void Kirby::handleInput(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // �������E����ִ�й���
        if (keyPressed->code == sf::Keyboard::Key::E)
        {
            attack(enemiesPtr);
        }
    }
}


// updateInput������ʵʱ���루ÿ֡��ⰴ��״̬��
void Kirby::updateInput() {
    // �����ƶ����� (A/D �������ҷ����)
    velocity.x = 0.f; // ÿ֡����ˮƽ�ٶ�
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -moveSpeed; // �����ƶ�
        facingRight = false;    // ������
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = moveSpeed;  // �����ƶ�
        facingRight = true;     // ������
    }

    // ��Ծ���� (�ո����W �����Ϸ����)
    // ֻ���ڵ���������Ծ��������ʱ������Ծ
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) && isGrounded) {
        if (!jumpPressed) { // �����ظ���Ծ������ֻ�ڰ���˲�䴥��
            jumpPressed = true;
            velocity.y = -jumpSpeed; // ʩ�����ϵ��ٶ�
            isGrounded = false;      // ���ٴ��ڵ���
        }
    }
    else {
        // �����Ծ���ɿ������� jumpPressed ��־
        jumpPressed = false;
    }

    // �¶����� (S �����·����)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        crouchPressed = true;
    }
    else {
        crouchPressed = false;
    }

    // �������� (J ��) - ֻ���ڵ�ǰ���ڹ���״̬ʱ���������¹���
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) && !isAttacking) {
        attack(enemiesPtr); // ������������
    }
}

void Kirby::updateLogic() {
    // --- �����߼��붯�� ---
    if (isAttacking) {
        // �����������ڽ���
        if (attackTimer.getElapsedTime().asSeconds() >= attackDuration) {
            isAttacking = false; // ������������
            std::cout << "Kirby ����������" << std::endl;
            // ����������ָ�Ĭ�϶���֡
            sprite.setTextureRect(standingRect);
        }
        else {
            // �ڹ�������ʱ���ڣ���ʾ��������֡
            sprite.setTextureRect(attackRect);
            // �����������֡�ĳߴ���վ��֡��ͬ��������Ҫ���������λ��
            // ���磺�����������������Ҫ����ƫ���Ա������Ĳ���
            // sprite.setPosition(position.x - (attackRect.width - standingRect.width) / 2.f, position.y);
            // ���蹥��ʱ�����鳯�򲻱䣬�����ڹ�����ʼʱ�����úó���
            // ������Ը��� faciongRight ����������Χ��λ�ã�ʹ���ڿ���ǰ��
            if (facingRight) {
                attackRangeShape.setPosition({ sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f + attackRadius / 2.f,
                    sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f });
            }
            else {
                attackRangeShape.setPosition({ sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f - attackRadius / 2.f,
                    sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f });
            }
        }

        // --- �����ڼ�����˺���� ---
        // ȷ�������б�����ҷǿ�
        if (enemiesPtr) {
            // ���������б�ʹ�õ������԰�ȫ���Ƴ������ĵ���
            for (auto it = enemiesPtr->begin(); it != enemiesPtr->end(); ) {
                if ((*it) && (*it)->getIsAlive()) { // ȷ�����˶�����Ч�Ҵ��
                    sf::FloatRect enemyBounds = (*it)->getGlobalBounds();
                    sf::Vector2f enemyCenter(enemyBounds.position.x + enemyBounds.size.x / 2.f,
                        enemyBounds.position.y + enemyBounds.size.y / 2.f);

                    // ��ȡ���ȹ�����Χ�����ĵ�
                    sf::Vector2f kirbyAttackCenter = attackRangeShape.getPosition(); // ������Χ��״��ԭ��������Ϊ����

                    // ����������ĵ����ȹ�����Χ���ĵľ���
                    float distance = std::sqrt(
                        std::pow(enemyCenter.x - kirbyAttackCenter.x, 2) +
                        std::pow(enemyCenter.y - kirbyAttackCenter.y, 2)
                    );

                    // ��������ڹ�����Χ��
                    if (distance < attackRadius) {
                        // �����ܵ��˺���ע�⣺Ϊ�˱���ÿ֡����ͬһ����������˺���
                        // �������Ҫһ�����ƣ�����һ��set�������ٱ��ֹ����Ѿ����еĵ��ˡ�
                        // ��ǰʵ�ֻ�ÿ֡������˺���ֱ�����������򹥻�������
                        std::cout << "���� " << (*it).get() << " �ڿ��ȵĹ�����Χ�ڣ��ܵ��˺���" << std::endl;
                        (*it)->takeDamage(20); // �����ܵ� 20 ���˺�

                        if (!(*it)->getIsAlive()) {
                            it = enemiesPtr->erase(it); // ����������������б����Ƴ�
                            continue; // �Ƴ��󣬼��������һ������
                        }
                    }
                }
                ++it; // �ƶ�����һ������
            }
        }
    }
    // --- �ǹ���״̬�µĶ������߼� ---
    else { // �����ǰû���ڹ���
        if (crouchPressed) {
            sprite.setTextureRect(crouchingRect); // ��ʾ�¶׶���֡
            // �¶�ʱ������Ҫ��������λ���Ա��ֵײ�����
            // sprite.setPosition(position.x, position.y + (standingRect.height - crouchingRect.height));
        }
        else {
            // Ĭ����ʾվ������֡
            sprite.setTextureRect(standingRect);
            // �ָ�����λ�ã����֮ǰ���¶׻򹥻�����������
            // sprite.setPosition(position);
        }
    }

    // ���ݳ���ת���飨��� facingRight Ϊ false��ˮƽ��ת���飩
    sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f });
    sprite.setScale(facingRight ? sf::Vector2f(1.f, 1.f) : sf::Vector2f(-1.f, 1.f));

    // --- ����ģ�� (��ʵ��) ---
    if (!isGrounded) {
        velocity.y += 980.f * static_cast<float>(0.016f); // ���������ٶ� (���� 980 ����/��^2��0.016 �� deltaTime ʾ��)
    }

    // --- �򵥵�����ײ��� (��ֹ������Ļ�ײ�) ---
    // ������� Y ����Ϊ 500 ���أ����ҿ��ȵĸ߶��� 50 ���أ�Ϊ���õײ�������棩
    // ������ sprite.getGlobalBounds().height ��̬��ȡ�߶�
    float groundLevel = 500.f - sprite.getGlobalBounds().size.y; // ����Y���� - ���ȸ߶�
    if (position.y >= groundLevel) {
        position.y = groundLevel; // �����ȹ̶��ڵ�����
        velocity.y = 0.f;         // ��ֱ�ٶȹ���
        isGrounded = true;        // ���Ϊ�ڵ�����
    }
}


// attack���������ȵĹ���
void Kirby::attack(std::vector<std::shared_ptr<Enemy>>* enemies) {
    if (!isAttacking) { // ֻ�е�ǰ���ڹ���״̬ʱ���������µĹ���
        isAttacking = true;
        attackTimer.restart(); // ���ù�����ʱ������ʼ����������ʱ
        std::cout << "Kirby ��ʼ������" << std::endl;
        // ���������ﲥ�Ź�����Ч������Ч��
        // ������ĵ����б�ָ�븳ֵ����Ա�������Ա� updateLogic ��ʹ��
        enemiesPtr = enemies;
    }
    // ʵ�ʵĵ����˺����Ͷ������Ž��� updateLogic �н���
}

// takeDamage�������ܵ��˺�
void Kirby::takeDamage(int amount) {
    if (isInvincible) {
        std::cout << "Kirby �޵У�δ�ܵ��˺���" << std::endl;
        return; // �޵�״̬�²�����
    }

    health -= amount; // �ܵ��̶� 10 ���˺�
    std::cout << "Kirby �ܵ��˺�����ǰ����ֵ��" << health << std::endl;

    if (health <= 0) {
        health = 0; // ����ֵ��Ϊ��
        isAlive = false; // ���ȱ�����
        std::cout << "Kirby �������ˣ���Ϸ������" << std::endl;
        // ���������ﴥ��������������Ϸ���������
    }
    else {
        isInvincible = true;       // �����޵�״̬
        invincibilityTimer.restart(); // �����޵м�ʱ��
        std::cout << "Kirby �����޵�״̬������ " << invincibilityDuration << " �롣" << std::endl;
    }
}

//�ж���ǽ�ڣ��ذ����ײ
void Kirby::handleObstacleCollisions(const std::vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        // ��鵱ǰ֡�Ƿ����ϰ��﷢���ص�
        if (std::optional<sf::FloatRect> intersection = boundingBox.findIntersection(obstacle.getGlobalBounds())) {
            // ��齻���Ƿ���Ч����Ⱥ͸߶ȶ�����0��
            if (intersection->size.x > 0 && intersection->size.y > 0) {
                // �ж���ˮƽ�ص���С�����Ǵ�ֱ�ص���С
                // �����ص�����С�ķ����ٶȹ��㲢�ƿ�
                if (intersection->size.x < intersection->size.y) {
                    // ˮƽ������ص���С��˵����ˮƽ��ײ
                    velocity.x = 0; // ֹͣˮƽ�ƶ�

                    // ��Kirby�ƿ������⿨���ϰ�����
                    if (boundingBox.position.x < obstacle.getGlobalBounds().position.x) {
                        // Kirby ���ϰ�����࣬�����ƿ�
                        position.x -= intersection->size.x;
                    }
                    else {
                        // Kirby ���ϰ����Ҳ࣬�����ƿ�
                        position.x += intersection->size.x;
                    }
                    std::cout << "Horizontal collision: velocity.x = 0." << std::endl;
                }
                else {
                    // ��ֱ������ص���С��˵���Ǵ�ֱ��ײ
                    velocity.y = 0; // ֹͣ��ֱ�ƶ�

                    // ��Kirby�ƿ������⿨���ϰ�����
                    if (boundingBox.position.y < obstacle.getGlobalBounds().position.y) {
                        // Kirby ���ϰ����Ϸ��������ƿ�
                        position.y -= intersection->size.y;
                        isGrounded = true; // ��ֱ����ײ�������壬��Ϊ�ŵ�
                    }
                    else {
                        // Kirby ���ϰ����·��������ƿ�
                        position.y += intersection->size.y;
                    }
                    std::cout << "Vertical collision: velocity.y = 0." << std::endl;
                }

                // ���¾���λ�ú���ײ���Է�ӳ�ƿ������λ��
                sprite.setPosition(position);
                boundingBox = sprite.getGlobalBounds();
            }
        }
    }
}

// handleGoalAndItemCollisions 
void Kirby::handleGoalAndItemCollisions(Goal& goal, std::vector<BoostItem>& boostItems) {
    if (goal.getIsAlive() && boundingBox.findIntersection(goal.getGlobalBounds())) {
        std::cout << "Kirby reached the goal! Game Won!" << std::endl;
        goal.setAlive(false);
    }

    for (auto it = boostItems.begin(); it != boostItems.end(); ) {
        if (it->getIsAlive() && boundingBox.findIntersection(it->getGlobalBounds())) {
            it->handleCollision(*this);
            it = boostItems.erase(it);
        }
        else {
            ++it;
        }
    }
}

