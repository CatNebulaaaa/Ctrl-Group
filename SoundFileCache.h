#pragma once
#include <map>
#include "SFML/Audio.hpp"
#include "IAudioProvider.h"
#include <string>
class SoundFileCache //用于管理音频文件资源的缓存类，func:加载&存储音频文件，避免重复加载
{
public://先声明
    SoundFileCache(); //构造函数
    ~SoundFileCache();//析构函数
    //使用公共方法getSound & getSong
    sf::Sound getSound(std::string filename) const;
    sf::Music* getSong(std::string filename) const;
private:
    static std::map<std::string, sf::SoundBuffer*> _sounds; //获取音效对象
    static std::map<std::string, sf::Music*> _music; //获取音乐对象指针

    //使用模板来扩充内容，编译器会创建一个于是用的任何数据类型匹配的方法
    //资源释放器
    struct SoundFileDeallocator //释放SoundBuffer资源的仿函数
    {
        void operator() (const std::pair<std::string, sf::SoundBuffer*>& p)
        {
            delete p.second;
        }
    };
    struct MusicFileDeallocator //释放Music资源Music资源的仿函数
    {
        void operator() (const std::pair<std::string, sf::Music*>& p)
        {
            delete p.second;
        }
    };
    template <typename T>
    struct Deallocator //模板化   //释放任意类型的资源
    {
        void operator() (const std::pair <std::string, T>& p)
        {
            delete p.second;
        }
    };
};
class SoundNotFoundExeception : public std::runtime_error
{    //自定义异常类，用于找不到音频文件时抛出异常
public:
    explicit SoundNotFoundExeception(const std::string& msg) : std::runtime_error(msg)
    {
        //自定义异常：文件名拼写修正+继承std::runtime_error
    }
};