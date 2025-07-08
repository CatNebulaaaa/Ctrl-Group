
#include"SFMLSoundProvider.h"
#include "SoundFileCache.h"
//sf::Sound����Ĭ�Ϲ��캯���������һ��sf::SoundBuffer���ܴ���
SFMLSoundProvider::SFMLSoundProvider()
{
    _currentSounds.resize(MAX_SOUND_CHANNELS);
}
//������Ч����
//problem��ÿ��ֻ�ܲ���һ����Ч�������������һ������������Ч
void SFMLSoundProvider::PlaySound(std::string filename)
{   //���Ŷ���Ч��֧�ֶ�ͨ��
    int availChannel = -1;
    for (int i = 0;i < MAX_SOUND_CHANNELS;i++)
    {   //if������Ҫ����һ���������������п���ͨ����ֱ���ҵ�һ��δ���ŵ�ͨ��
        if (_currentSounds[i].getStatus() != sf::SoundSource::Status::Playing)
        {
            availChannel = i;
            break; //�ҵ�һ������ͨ��������ѭ��
        }
    }
    if (availChannel != -1)
    {   //���ûͨ���ˣ����Ǵ�_soundFileCache�з�����õ��������󲢸���������
        try
        {
            _currentSounds[availChannel] = _soundFileCache.getSound(filename);
            // �� ʹ�ô���������Ч�ļ��������ظ�����
            _currentSounds[availChannel].play();
        }
        catch (SoundNotFoundExeception& snfe)
        {
            //ERROR,file wasnt found, should handle error here
            //Currently,this will simply mean nothing happens if an error occurs
        }
    }
}
//�������ֺ���
void SFMLSoundProvider::PlaySong(std::string filename, bool looping)
{   //֧�ֶ����������  //֧��ѭ��
    sf::Music* currentSong;
    try
    {
        currentSong = _soundFileCache.getSong(filename);
    }
    catch (SoundNotFoundExeception&)
    {
        //�����ļ�δ�ҵ���ֱ�ӷ���
        return;
    }
    //�Զ�ֹͣǰһ������
    if (_currentSongName != " ")  //_currentSongName���ٵ�ǰ����
    {
        try
        {    // �� ��ʽ���ţ��ʺϳ���Ƶ
            sf::Music* priorSong = _soundFileCache.getSong(_currentSongName);
            if (priorSong->getStatus() != sf::SoundSource::Status::Stopped)
            {
                priorSong->stop();  //ֹ֮ͣǰ������
            }
        }
        catch (SoundNotFoundExeception&)
        {
            //��Ĭ��������ļ�δ�ҵ���
            //Do nothing, this exception isn't dire.
            //It simply means the previous sound we were trying to stop wasn't located.
        }
        //DeepSeek���飺
        //_soundFileCache.releaseSong(_currentSongName);
    }
    _currentSongName = filename;
    currentSong->setLooping(looping);
    currentSong->play();
}
//ֹͣ������Ƶ
void SFMLSoundProvider::StopAllSounds()
{
    for (int i = 0;i < MAX_SOUND_CHANNELS;i++)
    {
        _currentSounds[i].stop();  //ֹͣ������Ч
    }
    if (_currentSongName != "")
    {
        sf::Music* currentSong = _soundFileCache.getSong(_currentSongName);
        if (currentSong->getStatus() == sf::SoundSource::Status::Playing)
        {
            currentSong->stop();  //ֹͣ��ǰ����
        }
    }
}
//�ж��Ƿ�����Ч���ڲ���
bool SFMLSoundProvider::IsSoundPlaying()
{
    for (int i = 0;i < MAX_SOUND_CHANNELS;i++)
    {
        if (_currentSounds[i].getStatus() == sf::SoundSource::Status::Playing) return true;
    }
    return false;
}
//�ж��Ƿ����������ڲ���
bool SFMLSoundProvider::IsSongPlaying()
{
    if (_currentSongName != "")
    {
        return _soundFileCache.getSong(_currentSongName)->getStatus() == sf::SoundSource::Status::Playing;
    }
    return false;
}