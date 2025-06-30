#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include<iostream>
// 假设Kirby和Enemy是自定义的游戏实体类


class Kirby {
private:
    sf::Sprite sprite;  // 用于表示Kirby的精灵
    sf::Texture texture;
    sf::FloatRect bounds;  // Kirby的碰撞框
    float speed;  // Kirby的移动速度

public:
    // 构造函数，初始化Kirby的相关属性
    Kirby(): sprite(texture) { //使用成员初始化列表
        // 假设加载一个默认的Kirby纹理图片，这里需根据实际纹理路径修改
        if (!texture.loadFromFile("C:\\Users\\bxmf7\\Desktop\\Image\\Kirby.png")) {
            // 处理纹理加载失败的情况，比如输出错误信息
            std::cerr << "Failed to load Kirby texture." << std::endl;
        }
        // 设置初始位置
        sprite.setPosition({ 100.f, 100.f });
        // 根据精灵尺寸初始化碰撞框，这里简单以精灵尺寸作为碰撞框尺寸
        bounds = sprite.getGlobalBounds();
        speed = 100.f;  // 设置移动速度
    }

    // 获取Kirby的精灵，用于渲染
    sf::Sprite getSprite() const {
        return sprite;
    }

    // 获取Kirby的碰撞框
    sf::FloatRect getBounds() const {
        return bounds;
    }

    // 移动Kirby
    void move(float dx, float dy) {
        sprite.move({ dx * speed, dy * speed });
        bounds.position += {dx * speed, dy* speed};
    }

    // 设置Kirby的位置
    void setPosition(float x, float y) {
        sprite.setPosition({ x, y });
        bounds.position.x = x;
        bounds.position.y = y;
    }
};
class Enemy {
public:
    sf::FloatRect getBounds() const {
        // 这里应返回敌人碰撞框的实际逻辑
        return sf::FloatRect();
    }
};

class CollisionManager {
public:
    // 检查碰撞
    void checkCollisions(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies) {
        sf::FloatRect kirbyBounds = kirby.getBounds();
        for (auto& enemy : enemies) {
            sf::FloatRect enemyBounds = enemy->getBounds();
            if (kirbyBounds.findIntersection(enemyBounds)) {
                // 这里调用相应的碰撞处理逻辑，暂未实现具体handleCollision方法
                // 可以根据实际需求添加碰撞后的响应，如扣血、反弹等
            }
        }
    }
};
