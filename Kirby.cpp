#include "Kirby.h"

// ���캯������ʼ����ɫ�Ļ������Ժ�״̬
Kirby::Kirby() {
    body.setSize(sf::Vector2f(16, 16)); // ���ý�ɫ������ײ���С���͵�ͼ���Ӵ�С��Ӧ
    body.setFillColor(sf::Color::Black); // ��ɫ��ɫ����Ϊ��ɫ����������
    velocity = sf::Vector2f(0.f, 0.f); // ��ʼ�ٶ�Ϊ��
    onGround = true;  // ��ʼʱ��ɫ�ڵ�����
    climbing = false;  // ��ʼʱ��ɫ����������״̬
    dead = false;      // ��ʼʱ��ɫδ����
    tileMap = nullptr; // ��ͼָ���ʼ��Ϊ�գ�֮��ͨ��setMap����
}

// �󶨵�ͼָ�룬�����ɫ������ѯ��ͼ��������
void Kirby::setMap(TileMap* map) {
    tileMap = map;
}

// ���ý�ɫλ�ã����Ͻ����꣩
void Kirby::setPosition(float x, float y) {
    body.setPosition({ x, y });
    velocity = sf::Vector2f(0.f, 0.f); // λ�ñ��ˣ��ٶȹ���

    fixInitialPosition(); // �����Զ���غ���������λ��
}

// ����������룬���ƽ�ɫˮƽ�ƶ��������ӣ���Ծ
void Kirby::handleInput() {
    if (dead) return; // �����ɫ������ֹͣ��������

    // ������£������ƶ����ٶ���Ϊ-60
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        velocity.x = -60.f;

    // �Ҽ����£������ƶ����ٶ���Ϊ60
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        velocity.x = 60.f;

    // ��ѯ��ǰ��ɫ�����������ͣ��ж��Ƿ���������
    TileType type = getCurrentTileType();

    if (type == TileType::ladder) {  // �����������
        climbing = true;              // ��Ϊ������״̬
        velocity.y = 0.f;             // ������ʱ��ֱ�ٶ������㣬��������Ӱ��

        // �ϼ����£����������ӣ��ٶ�Ϊ-40
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            velocity.y = -40.f;
        // �¼����£����������ӣ��ٶ�Ϊ40
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            velocity.y = 40.f;
    }
    else {
        climbing = false;  // ���������ϣ�ȡ��������״̬

        // �����ɫվ�ڵ����ϣ��ҿո�����£�������Ծ����ֱ�ٶ���Ϊ-150�����ϣ�
        if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            velocity.y = -150.f;
    }
}

// Ӧ���������ı䴹ֱ�ٶȣ��������������ӣ�
void Kirby::applyGravity(float dt) {
    if (climbing==0 && isOnGround() == 0) {
        velocity.y += 400.f * dt; // �������ٶ�Ϊ400������ʱ����dt���ٶ��ۼ�
    }
}

// ÿ֡���½�ɫ״̬
void Kirby::update(float dt) {
    if (dead) return;

    handleInput();     // ���� velocity.x / velocity.y
    applyGravity(dt);  // ģ���������޸� velocity.y

    body.move(velocity * dt); //ʵ���ƶ���ɫ

    checkCollisions(); // ����ͼ��ײ������λ�ã���ء���ˮ�ȣ�
}


// ���ƽ�ɫ���ε�����
void Kirby::draw(sf::RenderWindow& window) {
    window.draw(body);
}

// ��ѯ��ɫ�Ƿ�����
bool Kirby::isDead() const {
    return dead;
}

// ��ȡ��ɫ�������ڸ������ͣ������жϽ��»�ǰλ�û���
TileType Kirby::getCurrentTileType() {
    if (!tileMap) return TileType::empty; // û�󶨵�ͼ��Ĭ�Ͽո���

    // �����ɫ���ĵ��Ӧ�ĵ�ͼ������������
    int row = (int)((body.getPosition().y + body.getSize().y / 2) / tileMap->getTileHeight());
    int col = (int)((body.getPosition().x + body.getSize().x / 2) / tileMap->getTileWidth());

    // ���ظø��ӵ����ͣ�����桢���ӡ�ˮ�ȣ�
    return tileMap->getTileType(row, col);
}

// �жϽ�ɫ�ŵ��Ƿ���ڵ��涥��
bool Kirby::isOnGround() {
    if (!tileMap) return false; // δ�󶨵�ͼ����Ϊ���ڵ���

    float footY = body.getPosition().y + body.getSize().y; // �����ɫ�ŵ�y����
    int footRow = (int)(footY / tileMap->getTileHeight()); // ����ŵ׶�Ӧ��ͼ�����к�

    // ����ŵ����ұ߶�Ӧ���кţ������ɫ��������ӣ�
    int leftCol = (int)(body.getPosition().x / tileMap->getTileWidth());
    int rightCol = (int)((body.getPosition().x + body.getSize().x) / tileMap->getTileWidth());

    // lambda�����ж�ĳ�����Ƿ�Ϊ���涥��
    // �������ø��ǵ�����ӣ�������һ���ǵ��棬��ʾ����
    auto isGroundTop = [&](int r, int c) {
        if (tileMap->getTileType(r, c) != TileType::ground)
            return false;
        if (r == 0) // ��һ��û����һ��ֱ����Ϊ�Ƕ���
            return true;
        return tileMap->getTileType(r - 1, c) != TileType::ground;
        };

    // ֻҪ�ŵ���߻��ұߵĸ����ǵ��涥�㣬����Ϊ��ɫ���ڵ�����
    return (isGroundTop(footRow, leftCol) || isGroundTop(footRow, rightCol));
}

// ��ײ����봦��
void Kirby::checkCollisions() {
    if (!tileMap) return; // δ�󶨵�ͼ������

    // ���ж��Ƿ���ڵ��涥��
    if (isOnGround()) {
        onGround = true;  // ��ǽ�ɫ���ڵ���

        // ����ŵ������У���ȡ���е���߶�
        int footRow = (int)((body.getPosition().y + body.getSize().y) / tileMap->getTileHeight());
        float groundY = footRow * tileMap->getTileHeight();

        // ������ɫy��ʹ�ŵ�����λ�ڵ����ϣ���ֹ��ɫ�³��������
        body.setPosition({ body.getPosition().x, groundY - body.getSize().y });

        // �����ɫ�������ٶȣ����䣩�������㣬ֹͣ����
        if (velocity.y > 0) velocity.y = 0;
    }
    else {
        onGround = false; // ���ڵ�����
    }

    // ��ѯ��ǰ�������ͣ������������
    TileType type = getCurrentTileType();

    // ��ɫ�ȵ�ˮ�񣬴�������״̬
    if (type == TileType::water) {
        dead = true;
    }

    // ��ɫײ���ϰ���
    if (type == TileType::obstacle) {
        // �򵥴�����ɫ���˵���һ֡λ�ã�ֹͣ�ƶ�
        body.move(-velocity);
        velocity = sf::Vector2f(0.f, 0.f);
    }
}

// ���ؽ�ɫ��ǰ���Ͻ�����
sf::Vector2f Kirby::getPosition() {
    return body.getPosition();
}

void Kirby::fixInitialPosition() {
    if (!tileMap) return;

    if (isOnGround()) {
        onGround = true;
        velocity.y = 0;
        return;
    }

    const float dt = 0.016f;
    const int maxIterations = 100;

    int count = 0;
    velocity.y = 0; // ����̳��ϴε��ٶ�

    while (!isOnGround() && count < maxIterations) {
        applyGravity(dt);
        body.move(velocity * dt);
        checkCollisions(); // �������û������onGroundΪtrue
        count++;
    }

    velocity.y = 0;

    if (isOnGround()) {
        onGround = true; //�����غ�ȷ��������
    }
}


