#pragma once
#include "SFML/Audio.hpp"
#include "IAudioProvider.h"
class SFMLSoundProvider : public IAudioProvider
{
public:
    SFMLSoundProvider();  //构造函数，初始化内部成员（在.cpp写具体逻辑）

    //对接口IAudioProvider中的纯虚函数的实现(内容同IAudioProvider.h中一致)
    void PlaySound(std::string filename);
    void PlaySong(std::string filename, bool looping = false);
    void StopAllSounds();

    bool IsSoundPlaying();
    bool IsSongPlaying();
private: //音频资源
    static const int MAX_SOUND_CHANNELS = 5;  //存储音效的通道数，最多同时播放5个音效
    SoundFileCache _soundFileCache;
    //存储多个音效
    //此处sf::Sound必须关联sf::SoundBuffer才能使用，不能直接创建空对象
    std::vector<sf::Sound> _currentSounds;   //播放短小音效
    std::string _currentSongName;   //播放大型音频文件，支持流式播放（BGM）
};