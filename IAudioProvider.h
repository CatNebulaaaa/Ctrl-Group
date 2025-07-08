#pragma once  //��ֹͷ�ļ�����ΰ���
#include "SFML/Audio.hpp"  //Ԥ����ͷ�ļ������ܰ���ͨ�ÿ⡢SFMLͷ�ļ�
#include <string.h>
#include<filesystem>
class IAudioProvider
{
public:
    virtual ~IAudioProvider() {}
    //������������ȷ��ͨ������ָ��ɾ���������ʱ���ܹ���ȷ���������������������ֹ��Դй¶

    virtual void PlaySound(const std::filesystem::path& filename) = 0;  //����һ������Ч ����ť�������
    virtual void PlaySong(const std::filesystem::path& filename, bool looping) = 0;  //���ű�������
    //looping ��ʾ�Ƿ�ѭ������
    virtual void StopAllSounds() = 0;  //ֹͣ�������ڲ��ŵ���Ƶ

    //�ж���Чor�����Ƿ����ڲ���
    virtual bool IsSoundPlaying() = 0;
    virtual bool IsSongPlaying() = 0;
};