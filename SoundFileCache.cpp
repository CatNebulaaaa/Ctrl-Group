#include "SoundFileCache.h"
#include "IAudioProvider.h"
#include <filesystem>
#include <memory>
#include<mutex>

SoundFileCache::SoundFileCache() {}  //空构造函数
SoundFileCache::~SoundFileCache()
{   //析构时，释放所有音效&音乐资源
    std::for_each(_sounds.begin(), _sounds.end(), Deallocator<sf::SoundBuffer*>());
    std::for_each(_music.begin(), _music.end(), Deallocator<sf::Music*>());
}
sf::Sound SoundFileCache::getSound(std::string soundName) const  //获取一个表示声音文件名的字符串
{   //查找音效是否已加载
    std::map<std::string, sf::SoundBuffer*>::iterator itr = _sounds.find(soundName);
    if (itr == _sounds.end())
    {   //创建新的音效缓冲区
        sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
        //尝试加载文件
        if (!soundBuffer->loadFromFile(soundName))
        {
            delete soundBuffer;
            throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::gerSound");
        }
        //插入到缓存中
        std::map<std::string, sf::SoundBuffer*>::iterator res = _sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundName, soundBuffer)).first;
        //创建并返回音效对象
        sf::Sound sound;
        sound.setBuffer(*soundBuffer);
        return sound;
    }
    else //如果已缓存                          //后续请求相同的音效
    {   //使用缓存的音效缓冲区创建音效对象
        sf::Sound sound;
        sound.setBuffer(*itr->second);        // ← 使用此来缓存音效文件，避免重复加载（对象池）
        return sound;
    }
    throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::gerSound");
}
sf::Music* SoundFileCache::getSong(std::string soundName) const
{   //查找音乐是否已加载
    std::map<std::string, sf::Music*>::iterator itr = _music.find(soundName);
    if (itr == _music.end()) //if没找到
    {   //创建新的音乐对象
        sf::Music* song = new sf::Music();
        //尝试加载文件
        if (!song->openFromFile(soundName))
        {
            delete song;
            throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::getSong");
        }
        else
        {   //插入到缓存中并返回
            std::map<std::string, sf::Music*>::iterator res = _music.insert(std::pair<std::string, sf::Music*>(soundName, song)).first;
            return res->second;
        }
    }
    else  //如果已缓存
    {
        return itr->second;
    }
    throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::getSong");
}

std::map<std::string, sf::SoundBuffer*> SoundFileCache::_sounds;
std::map<std::string, sf::Music*> SoundFileCache::_music;
//对象池模式：1.缓存音效&音乐：避免重复加载
          // 2.延迟加载：只在第一次请求时加载资源
          // 3.统一管理：集中管理所有音频资源，便于释放