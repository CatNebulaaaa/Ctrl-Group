//缓存：一种将常用信息储存在更快储存器中的行为
//添加音频要权衡内存
//IAudioProvider.h
#pragma once  //防止头文件被多次包含
#include "stdafx.h"  //预编译头文件，可能包含通用库、SFML头文件
#include <string.h>
class IAudioProvider
{
public:
    virtual ~IAudioProvider() {}  
    //虚析构函数，确保通过基类指针删除子类对象时，能够正确调用子类的析构函数，防止资源泄露

    virtual void PlaySound(std::string filename) = 0;  //播放一次性音效 （按钮点击音）
    virtual void PlaySong(std::string filename, bool looping) = 0;  //播放背景音乐
    //looping 表示是否循环播放
    virtual void StopAllSounds() = 0;  //停止所有正在播放的音频

    //判断音效or歌曲是否正在播放
    virtual bool IsSoundPlaying() = 0;
    virtual bool IsSongPlaying() = 0;
};
//SFMLSoundProvider.h  服务类 实现IAudioProvider接口
#pragma once
#include "stdafx.h"
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
    sf::Sound _currentSounds[MAX_SOUND_CHANNELS];   //播放短小音效
    sf::Music _currentSongName;   //播放大型音频文件，支持流式播放（BGM）
};
//SFMLSoundProvider.cpp
#include"stdafx.h
#include"SFMLSoundProvider.h"
#include "SoundFileCache.h"
SFMLSoundProvider::SFMLSoundProvider() : _currentSongName("")
{
}
//播放音效函数
//problem：每次只能播放一个音效，解决方法：用一个容器储存音效
void SFMLSoundProvider::PlaySound(std::string filename) 
{   //播放短音效，支持多通道
    int availChannel = -1;
    for (int i=0;i<MAX_SOUND_CHANNELS;i++)
    {   //if我们想要播放一个声音，遍历所有可用通道，直到找到一个未播放的通道
        if(_currentSounds[i].getStatus() != sf::Sound::Playing)
        {
            availChannel = i;
            break; //找到一个空闲通道，跳出循环
        }
    }
    if(availChannel != -1)
    {   //如果没通道了，我们从_soundFileCache中分配可用的声音对象并告诉它播放
        try
        {
            _currentSounds[availChannel] = _soundFileCache.getSound(filename);
                                         // ↑ 使用此来缓存音效文件，避免重复加载
            _currentSounds[availChannel].play();
        }
        catch(SoundNotFoundExeception& snfe)
        {
            //ERROR,file wasnt found, should handle error here
            //Currently,this will simply mean nothing happens if an error occurs
        }
    }
}
//播放音乐函数
void SFMLSoundProvider::PlaySong(std::string filename,bool looping)
{   //支持多个并发声音  //支持循环
    sf::Music * currentSong;
    try
    {
        currentSong = _soundFileCache.getSong(filename);
    }
    catch(SoundNotFooundExeception&)
    {
        //音乐文件未找到，直接返回
        return;
    }
    //自动停止前一首音乐
    if(_currentSongName != "")  //_currentSongName跟踪当前音乐
    {
        try
        {    // ↓ 流式播放，适合长音频
            sf::Music* priorSong = _soundFileCache.getSong(_currentSongName);
            if(priorSong->getStatus() != sf::Sound::Stopped)
            {
                priorSong->stop();  //停止之前的音乐
            }
        }
        catch(SoundNotFoundExeception&)
        {
            //静默处理错误（文件未找到）
            //Do nothing, this exception isn't dire.
            //It simply means the previous sound we were trying to stop wasn't located.
        }
        //DeepSeek建议：
        //_soundFileCache.releaseSong(_currentSongName);
    }
    _currentSongName = filename;
    currentSong->setLoop(looping);
    currentSong->play();
}
//停止所有音频
void SFMLSoundProvider::StopAllSounds() 
{
    for (int i=0;i<MAX_SOUND_CHANNELS;i++)
    {    
        _currentSounds[i].stop();  //停止所有音效
    }
    if (_currentSongName != "")
    {
        sf::Music * currentSong = _soundFileCache.getSong(_currentSongName);
        if(currentSong->getStatus() == sf::Sound::Playing)
        {
            currentSong->stop();  //停止当前音乐
        }
    }
}
//判断是否有音效正在播放
bool SFMLSoundProvider::IsSoundPlaying()
{
    for (int i=0;i<MAX_SOUND_CHANNELS;i++)
    {   
        if(_currentSounds[i].getStatus() == sf::Sound::Playing) return true;
    }
    return false;
}
//判断是否有音乐正在播放
bool SFMLSoundProvider::IsSongPlaying()
{
    if(_currentSongName != "")
    {
        return _soundFileCache.getSong(_currentSongName)->getStatus() == sf::Music::Playing;
    }
    return false;
}

//SoundFileCache.h
//此类不旨在继承的类
//提供音频文件的集中管理/缓存；  避免统一音频文件的重复加载；  
//统一管理音频资源的生命周期；  提供一致的错误处理机制
class SoundFileCache //用于管理音频文件资源的缓存类，func:加载&存储音频文件，避免重复加载
{
public://先声明
    SoundFileCache(); //构造函数
    ~SoundFileCache();//析构函数
    //使用公共方法getSound & getSong
    sf::Sound getSound(std::string filename);
    sf::Music* getSong(std::string filename);
private:
    static std::map<std::string, sf::SoundBuffer*> _sounds; //获取音效对象
    static std::map<std::string, sf::Music*> _music; //获取音乐对象指针

    //使用模板来扩充内容，编译器会创建一个于是用的任何数据类型匹配的方法
    //资源释放器
    struct SoundFileDeallocator //释放SoundBuffer资源的仿函数
    {
        void operator() (const std::pair<std::string, sf::SoundBuffer*> & p)
        {
            delete p.second;
        }
    };
    struct MusicFileDeallocator //释放Music资源Music资源的仿函数
    {
        void operator() (const std::pair<std::string, sf::Music*> & p)
        {
            delete p.second;
        }
    }
    template <typename T>
    struct Deallocator //模板化   //释放任意类型的资源
    {
        void operator() (const std::pair <std::string, T> & p)
        {
            delete p.second;
        }
    };
};
class SoundNotFoundExeception : public std::runtime_error
{    //自定义异常类，用于找不到音频文件时抛出异常
public:
    SoundNotFoundExeception(std::string const& msg) : std::runtime_error(msg)
    {}
};
//SoundFileCache.cpp
#include "stdafx.h"
#include "SoundFileCache.h"
SoundFileCache::SoundFileCache() {}
SoundFileCache::~SoundFileCache()
{
    std::for_each(_sounds.begin(), _sounds.end(), Deallocator<sf::SoundBuffer*>());
    std::for_each(_music.begin(), _music.end(), Deallocator<sf::Music*>());
}
sf::Sound SoundFileCache::getSound(std::string soundName) const
{
    std::map<std::string, sf::SoundBuffer*>::iterator itr = _sounds.find(soundName);
    if(itr == _sounds.end())
    {
        sf::SoundBuffer *soundBuffer = new sf::SoundBuffer();
        if(!soundBuffer->loadFromFile(soundName))
        {
            delete soundBuffer;
            throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::gerSound");
        }
        std::map<std::string, sf::SoundBuffer*>::iterator res=_sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundName, soundBuffer)).first;

        sf::Sound sound;
        sound.setBuffer(*soundBuffer);
        return sound;
    }
    else
    {
        sf::Sound sound;
        sound.setBuffer(*itr->second);
        return sound;
    }
    throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::gerSound");
}
sf::Music* SoundFileCache::getSong(std::string soundName) const
{
    std::map<std::string, sf::Music*>::iterator itr = _music.find(soundName);
    if(itr == _music.end())
    {
        sf::Music * song = new sf::Music();
        if(!song->openFromFile(soundFile))
        {
            
        }
    }
}