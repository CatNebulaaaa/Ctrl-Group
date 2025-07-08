#pragma once  //防止头文件被多次包含
#include "SFML/Audio.hpp"  //预编译头文件，可能包含通用库、SFML头文件
#include <string.h>
#include<filesystem>
class IAudioProvider
{
public:
    virtual ~IAudioProvider() {}
    //虚析构函数，确保通过基类指针删除子类对象时，能够正确调用子类的析构函数，防止资源泄露

    virtual void PlaySound(const std::filesystem::path& filename) = 0;  //播放一次性音效 （按钮点击音）
    virtual void PlaySong(const std::filesystem::path& filename, bool looping) = 0;  //播放背景音乐
    //looping 表示是否循环播放
    virtual void StopAllSounds() = 0;  //停止所有正在播放的音频

    //判断音效or歌曲是否正在播放
    virtual bool IsSoundPlaying() = 0;
    virtual bool IsSongPlaying() = 0;
};