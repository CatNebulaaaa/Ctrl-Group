#pragma once
#include "SFML/Audio.hpp"
#include "IAudioProvider.h"
class SFMLSoundProvider : public IAudioProvider
{
public:
    SFMLSoundProvider();  //���캯������ʼ���ڲ���Ա����.cppд�����߼���

    //�Խӿ�IAudioProvider�еĴ��麯����ʵ��(����ͬIAudioProvider.h��һ��)
    void PlaySound(std::string filename);
    void PlaySong(std::string filename, bool looping = false);
    void StopAllSounds();

    bool IsSoundPlaying();
    bool IsSongPlaying();
private: //��Ƶ��Դ
    static const int MAX_SOUND_CHANNELS = 5;  //�洢��Ч��ͨ���������ͬʱ����5����Ч
    SoundFileCache _soundFileCache;
    //�洢�����Ч
    sf::Sound _currentSounds[MAX_SOUND_CHANNELS];   //���Ŷ�С��Ч
    sf::Music _currentSongName;   //���Ŵ�����Ƶ�ļ���֧����ʽ���ţ�BGM��
};