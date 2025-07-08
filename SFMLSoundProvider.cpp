
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
    for (int i = 0;i < MAX_SOUND_CHANNELS;i++)
    {   //if我们想要播放一个声音，遍历所有可用通道，直到找到一个未播放的通道
        if (_currentSounds[i].getStatus() != sf::Sound::Playing)
        {
            availChannel = i;
            break; //找到一个空闲通道，跳出循环
        }
    }
    if (availChannel != -1)
    {   //如果没通道了，我们从_soundFileCache中分配可用的声音对象并告诉它播放
        try
        {
            _currentSounds[availChannel] = _soundFileCache.getSound(filename);
            // ↑ 使用此来缓存音效文件，避免重复加载
            _currentSounds[availChannel].play();
        }
        catch (SoundNotFoundExeception& snfe)
        {
            //ERROR,file wasnt found, should handle error here
            //Currently,this will simply mean nothing happens if an error occurs
        }
    }
}
//播放音乐函数
void SFMLSoundProvider::PlaySong(std::string filename, bool looping)
{   //支持多个并发声音  //支持循环
    sf::Music* currentSong;
    try
    {
        currentSong = _soundFileCache.getSong(filename);
    }
    catch (SoundNotFooundExeception&)
    {
        //音乐文件未找到，直接返回
        return;
    }
    //自动停止前一首音乐
    if (_currentSongName != "")  //_currentSongName跟踪当前音乐
    {
        try
        {    // ↓ 流式播放，适合长音频
            sf::Music* priorSong = _soundFileCache.getSong(_currentSongName);
            if (priorSong->getStatus() != sf::Sound::Stopped)
            {
                priorSong->stop();  //停止之前的音乐
            }
        }
        catch (SoundNotFoundExeception&)
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
    for (int i = 0;i < MAX_SOUND_CHANNELS;i++)
    {
        _currentSounds[i].stop();  //停止所有音效
    }
    if (_currentSongName != "")
    {
        sf::Music* currentSong = _soundFileCache.getSong(_currentSongName);
        if (currentSong->getStatus() == sf::Sound::Playing)
        {
            currentSong->stop();  //停止当前音乐
        }
    }
}
//判断是否有音效正在播放
bool SFMLSoundProvider::IsSoundPlaying()
{
    for (int i = 0;i < MAX_SOUND_CHANNELS;i++)
    {
        if (_currentSounds[i].getStatus() == sf::Sound::Playing) return true;
    }
    return false;
}
//判断是否有音乐正在播放
bool SFMLSoundProvider::IsSongPlaying()
{
    if (_currentSongName != "")
    {
        return _soundFileCache.getSong(_currentSongName)->getStatus() == sf::Music::Playing;
    }
    return false;
}