#include "SoundFileCache.h"
#include "IAudioProvider.h"
#include <filesystem>
#include <memory>
#include<mutex>

SoundFileCache::SoundFileCache() {}  //�չ��캯��
SoundFileCache::~SoundFileCache()
{   //����ʱ���ͷ�������Ч&������Դ
    std::for_each(_sounds.begin(), _sounds.end(), Deallocator<sf::SoundBuffer*>());
    std::for_each(_music.begin(), _music.end(), Deallocator<sf::Music*>());
}
sf::Sound SoundFileCache::getSound(std::string soundName) const  //��ȡһ����ʾ�����ļ������ַ���
{   //������Ч�Ƿ��Ѽ���
    std::map<std::string, sf::SoundBuffer*>::iterator itr = _sounds.find(soundName);
    if (itr == _sounds.end())
    {   //�����µ���Ч������
        sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
        //���Լ����ļ�
        if (!soundBuffer->loadFromFile(soundName))
        {
            delete soundBuffer;
            throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::gerSound");
        }
        //���뵽������
        std::map<std::string, sf::SoundBuffer*>::iterator res = _sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundName, soundBuffer)).first;
        //������������Ч����
        sf::Sound sound;
        sound.setBuffer(*soundBuffer);
        return sound;
    }
    else //����ѻ���                          //����������ͬ����Ч
    {   //ʹ�û������Ч������������Ч����
        sf::Sound sound;
        sound.setBuffer(*itr->second);        // �� ʹ�ô���������Ч�ļ��������ظ����أ�����أ�
        return sound;
    }
    throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::gerSound");
}
sf::Music* SoundFileCache::getSong(std::string soundName) const
{   //���������Ƿ��Ѽ���
    std::map<std::string, sf::Music*>::iterator itr = _music.find(soundName);
    if (itr == _music.end()) //ifû�ҵ�
    {   //�����µ����ֶ���
        sf::Music* song = new sf::Music();
        //���Լ����ļ�
        if (!song->openFromFile(soundName))
        {
            delete song;
            throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::getSong");
        }
        else
        {   //���뵽�����в�����
            std::map<std::string, sf::Music*>::iterator res = _music.insert(std::pair<std::string, sf::Music*>(soundName, song)).first;
            return res->second;
        }
    }
    else  //����ѻ���
    {
        return itr->second;
    }
    throw SoundNotFoundExeception(soundName + "was not found in call to SoundFileCache::getSong");
}

std::map<std::string, sf::SoundBuffer*> SoundFileCache::_sounds;
std::map<std::string, sf::Music*> SoundFileCache::_music;
//�����ģʽ��1.������Ч&���֣������ظ�����
          // 2.�ӳټ��أ�ֻ�ڵ�һ������ʱ������Դ
          // 3.ͳһ�������й���������Ƶ��Դ�������ͷ�