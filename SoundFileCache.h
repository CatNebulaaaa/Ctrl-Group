#pragma once
#include <map>
#include "SFML/Audio.hpp"
#include "IAudioProvider.h"
#include <string>
class SoundFileCache //���ڹ�����Ƶ�ļ���Դ�Ļ����࣬func:����&�洢��Ƶ�ļ��������ظ�����
{
public://������
    SoundFileCache(); //���캯��
    ~SoundFileCache();//��������
    //ʹ�ù�������getSound & getSong
    sf::Sound getSound(std::string filename) const;
    sf::Music* getSong(std::string filename) const;
private:
    static std::map<std::string, sf::SoundBuffer*> _sounds; //��ȡ��Ч����
    static std::map<std::string, sf::Music*> _music; //��ȡ���ֶ���ָ��

    //ʹ��ģ�����������ݣ��������ᴴ��һ�������õ��κ���������ƥ��ķ���
    //��Դ�ͷ���
    struct SoundFileDeallocator //�ͷ�SoundBuffer��Դ�ķº���
    {
        void operator() (const std::pair<std::string, sf::SoundBuffer*>& p)
        {
            delete p.second;
        }
    };
    struct MusicFileDeallocator //�ͷ�Music��ԴMusic��Դ�ķº���
    {
        void operator() (const std::pair<std::string, sf::Music*>& p)
        {
            delete p.second;
        }
    };
    template <typename T>
    struct Deallocator //ģ�廯   //�ͷ��������͵���Դ
    {
        void operator() (const std::pair <std::string, T>& p)
        {
            delete p.second;
        }
    };
};
class SoundNotFoundExeception : public std::runtime_error
{    //�Զ����쳣�࣬�����Ҳ�����Ƶ�ļ�ʱ�׳��쳣
public:
    explicit SoundNotFoundExeception(const std::string& msg) : std::runtime_error(msg)
    {
        //�Զ����쳣���ļ���ƴд����+�̳�std::runtime_error
    }
};