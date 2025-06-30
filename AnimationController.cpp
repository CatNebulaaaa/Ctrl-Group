#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

class AnimationController {
private:
    sf::Texture* spriteSheet;
    std::map<std::string, std::vector<sf::IntRect>> animations;
    std::string currentAnimationName;
    float frameTime;
    float timeElapsed;
    int currentFrameIndex;
    bool loopAnimation;

public:
    // 添加动画
    void addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames, float frameDuration) {
        animations[name] = frames;
        frameTime = frameDuration;
    }

    // 播放动画
    void play(const std::string& name, bool loop = true) {
        currentAnimationName = name;
        currentFrameIndex = 0;
        timeElapsed = 0.0f;
        loopAnimation = loop;
    }

    // 更新动画
    void update(float deltaTime) {
        if (animations.find(currentAnimationName) != animations.end()) {
            timeElapsed += deltaTime;
            if (timeElapsed >= frameTime) {
                timeElapsed -= frameTime;
                currentFrameIndex++;
                if (currentFrameIndex >= static_cast<int>(animations[currentAnimationName].size())) {
                    if (animations[currentAnimationName].size() > 0) {
                        if (loopAnimation) {
                            currentFrameIndex = 0;
                        }
                        else {
                            currentFrameIndex = animations[currentAnimationName].size() - 1;
                        }
                    }
                }
            }
        }
    }

    // 获取当前动画帧的纹理矩形
    sf::IntRect getCurrentFrame() const {
        auto it = animations.find(currentAnimationName);
        if (it != animations.end() && currentFrameIndex <static_cast<int>(it->second.size())){
            return it->second[currentFrameIndex];
        }
        return sf::IntRect();
    }
};