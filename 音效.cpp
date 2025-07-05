//以松散耦合的方式为游戏添加音效
//耦合：两个或多个类之间的关系
sf::SoundBuffer mySound;    //SoundBuffer的资源密集程度较高，应被缓存来维持游戏性能
mySound.LoadFromFile("MySound.wav");
if (mySound)
{
    sf::soundsound; 
    sound.setBuffer (mySound); 
    sound.play();
}
//The Gang of Four网站的设计模式值得学习
//P.S. 学习设计模式要警惕过度设计所有东西

//IAudioProvider.h   音频提供者接口（抽象基类）
#pragma once  //防止头文件被多次包含
#include "stdafx.h"  //预编译头文件，可能包含通用库、SFML头文件
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
        //e.p.假设有两个音频系统实现，可以使用这样的接口
        class FAudioProvider : public IAudioProvider{...};
        class DAudioProvider : public IAudioProvider{...};
        void Game::InitAudio()
        {
            m_audio = new FAudioProvider();
            m_audio->PlaySound("bgm.mp3",true);
        }
        //Advantage: 即使换音频引擎，也不用改其他业务逻辑代码

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
    sf::SoundBuffer _soundBuffer;  //表示构成歌曲波形的实际比特&字节
    //SFML中用于加载并储存音效数据
    sf::Sound _sound;   //播放短小音效
    sf::Music _music;   //播放大型音频文件，支持流式播放（BGM）
};

//SFMLSoundProvider.cpp
#include"stdafx.h
#include"SFMLSoundProvider.h"
SFMLSoundProvider::SFMLSoundProvider()
{
    //构造函数中，将音效音量设为100%
    _sound.setVolume(100.0f);
}
//播放音效函数
//problem：每次只能播放一个音效，解决方法：用一个容器储存音效
void SFMLSoundProvider::PlaySound(std::string filename) 
{
    if(_soundBuffer.getDuration() == 0)  
    //判断_soundBuffer是否为零长度，if是，使用提供的文件名从文件中加载SoundBuffer，then检查我们的声音是否正在播放
        _soundBuffer.loadFromFile(filename);
    if(_sound.getStatus() == sf::Sound::Playing)  //如果音效正在播放，则1.先暂停
        _sound.stop();
    _sound.setBuffer(_soundBuffer); //2.设置缓冲区到_sound对象
    _sound.play();  //3.再播放
    //失败处理
    if(!_music.openFromFile(filename))
    {
        std::cerr << "Failed to load music:" << filename << std::endl;
        return;
    }
} 
//播放音乐函数
void SFMLSoundProvider::PlaySong(std::string filename,bool looping)
{
    _music.openFromFile(filename); //打开音乐文件，大文件用sf::Music以流方式读取
    _music.setLoop(looping); //检查是否循环
    _music.play(); //播放音乐
    //失败处理
    if(!_music.openFromFile(filename))
    {
        std::cerr << "Failed to load music:" << filename << std::endl;
        return;
    }
}
//停止所有音频
void SFMLSoundProvider::StopAllSounds() 
{
    if(_sound.getStatus() == sf::Sound::Playing)  _sound.stop(); //停止音效
    if(_sound.getStatus() == sf::Sound::Playing) _music.stop();  //停止音乐
}
//判断是否有音效正在播放
bool SFMLSoundProvider::IsSoundPlaying()
{
    return _sound.getStatus() == sf::Sound::Playing;
}
//判断是否有音乐正在播放
bool SFMLSoundProvider::IsSongPlaying()
{
    return _music.getStatus() == sf::MusicPlaying;
}

//ServiceLocator.h  使该服务对外界可用
//这个模式，被认为是“反依赖注入”，可能会导致全局状态污染，不宜测试和维护
#pragma once
#include "stdafx.h"
#include "IAudioProvider.h"
class ServiceLocator
{
public://方法1
    static IAudioProvider* getAudio() {return _audioProvider;} //getAudio()获取当前全局注册的音频服务
    static void RegisterServiceLocator(IAudioProvider *provider) //注册一个音频服务实例，供后续使用
        _audioProvider = provider; //静态成员变量，保存音频服务的指针
private://方法2
    static IAudioProvider * _audioProvider; //存储一个指向IAudioProvider的指针，以便全局访问音频服务
};
//ServiceLocator.cpp
#include "stdafx.h"
#include "ServiceLocator.h"
IAudioProvider* ServiceLocator::_audioProvider = NULL;  //定义_audioProvider静态变量，它需要提供一个实现
//使用示例
#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"
void InitAudio()
{
    ServiceLocator::RegisterAudioProvider(new SFMLSoundProvider()); //注册
}
void PlayButtonClickSound()
{
    ServiceLocator::getAudio()->PlaySound("click.wav"); //使用
}

//Game.cpp   在Game::Start()顶部添加以下代码：
SFMLSoundProvider soundProvider;
   ServiceLocator::RegisterServiceLocator(&soundProvider);
ServiceLocator::getAudio()->PlaySong("audi/Soundtrack.ogg",true); //在游戏开始时，立刻播放（循环播放）
//这样便正式注册了一个AudioProvider “初始化器”

    //如何实现在代码中任何位置，播放歌曲/声音
    ServiceLocator::getAudio()->PlaySound("audio/kaboom.wav");
    ServiceLocator::getAudio()->PlayMusic("audio/song.ogg");

//在MainMenu.cpp中GetMenuResponse()中，if有鼠标点击，添加以下代码停止标题音轨的播放
    if(ServiceLocator::getAudio()->IsSongPlaying())
    {
        ServiceLocator::getAudio()->StopAllSounds();
    }

    //在实际项目中使用在相应想要有音效的点
    ServiceLocator::getAudio()->PlaySound("audio/kaboom.wav");

//FMOD更全的库
//FModSoundProvider.h
#pragma once
#include "IAudioProvider.h"
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"
class FModSoundProvider : public IAudioProvider
{
public:
    FModSoundProvider();
    virtual ~FModSoundProvider();

    virtual void PlaySound(std::string filename);
    virtual void PlaySong(std::string filename, bool looping);
    virtual void StopAllSounds();
    
    virtual bool IsSoundPlaying();
    virtual bool IsSongPlaying();
private:
    void InitFMOD();
    FMOD::System * _system;
    FMOD::Sound * _sound;
    FMOD::Channel * _soundChannel;
    FMOD::Sound * _song;
    FMOD::Channel * _musicChannel;
    void ERRCHECK(FMOD_RESULT);
};
//FModSoundProvider.cpp
#include "stdafx.h"
#include "FModSoundProvider.h"
#include<iostream>
FModSoundProvider::FModSoundProvider() : 
    _system(nullptr),_sound(nullptr),_song(nullptr),
    _soundChannel(nullptr),_musicChannel(nullptr)
{
    InitFMOD(); //初始化FMOD系统
}
FModSoundProvider::~FModSoundProvider()
{
    if(_sound != NULL)  _sound->release();
    if(_song != NULL)  _song->release();
    if(_system != NULL)  _system->release();
}
void FModSoundProvider::PlaySound(std::string filename)
{
    ERRCHECK(_system->createSound(filename.c_str(),FMOD_DEFAULT,0,&_sound));
    ERRCHECK(_system->playSound(FMOD_CHANNEL_REUSE, _sound, false, &_soundChannel));
                              // ↑ 用此，复用之前的通道，避免创建过多通道
}
void FModSoundProvider::PlaySong(std::string filename, bool looping)
{
    if(looping) 
        _system->createSound(filename.c_str(), FMOD_LOOP_NORMAL, 0, &_song);
    else  
        _system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &_song);

    _system->playSound(FMOD_CHANNEL_REUSE, _song, false, &_musicChannel);
                     // ↑复用音乐通道
}
void FModSoundProvider::StopAllSounds()
{
    int channelIndex;
    FMOD_RESULT result;        //显式声明局部变量result和nextChannel，代码清晰
    FMOD::Channel *nextChannel;

    for (channelIndex = 0; channelIndex < 100;channelIndex++)
    {
        result = _system->getChannel(channelIndex, &nextChannel);
        if ((result == FMOD_OK) && (nextChannel != NULL))  
            nextChannel->stop();
    }
}
bool FModSoundProvider::IsSoundPlaying()
{
    bool result;
    _soundChannel -> isPlaying(&result);
    //直接返回bool结果，不再检查_soundChannel是否为空
    //Risk：if _soundChannel未初始化，会直接崩溃
    return result;
}
bool FModSoundProvider::IsSongPlaying()
{
    bool result;
    _musicChannel -> isPlaying(&result);
    return result;
}
void FModSoundProvider::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)  
    {
        //移除了错误信息打印std::cerr，仅简单退出程序
        exit(-1);
        //Problem: 不利于调试，无法知道具体错误原因
    }
}
void FModSoundProvider::InitFMOD()
{
    FMOD_RESULT result;
    unsigned int version;
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    char name[256];

    //创建一个系统对象，并初始化
    result = FMOD::System_Create(&_system);
    ERRCHECK(result);
    //版本检查
    result = _system->getVersion(&version);
    ERRCHECK(result);
    if(version < FMOD_VERSION)
    {
        printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
        return; //此处未处理错误，might继续运行
    }
    //驱动检测与配置
    result = _system->getNumDrivers(&numdrivers);
    ERRCHECK(result);

    if(numdrivers == 0)
    {
        result = _system->setOutput(FMOD_OUTPUTTYPE_NOSOUND); //无设备时静音
        ERRCHECK(result);
    }
    else
    {
        result = _system->getDriverCaps(0,&caps,0,&speakermode);
        ERRCHECK(result);
        //set the user selected speaker mode.
        result = _system->setSpeakerMode(speakermode);
        ERRCHECK(result);

        if(caps & FMOD_CAPS_HARDWARE_EMULATED)
        {
            result = _system->setDSPBufferSize(1024,10);
            ERRCHECK(result);
        }
        result = _system->getDriverInfo(0,name,256,0);
        ERRCHECK(result);
        if(strstr(name, "SigmaTel"))
        {
            result = _system->setSoftwareFormat(48000,FMOD_SOUND_FORMAT_PCMFLOAT,0,0,FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(result);
        }
    }
    //初始化系统（失败时降级为立体声模式）
    result = _system->init(100,FMOD_INIT_NORMAL,0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
        result = _system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);

        result = _system->init(100,FMOD_INIT_NORMAL,0);
    }
    ERRCHECK(result);
}








//ChatGPT改进后
#include "stdafx.h"
#include "FModSoundProvider.h"
#include <iostream> //错误输出std::cerr

FModSoundProvider::FModSoundProvider() : 
    _system(nullptr), _sound(nullptr), _song(nullptr), 
    _soundChannel(nullptr), _musicChannel(nullptr)     //初始化成员变量为nullptr
{
    InitFMOD(); //初始化FMOD系统
}

FModSoundProvider::~FModSoundProvider()  //释放所有FMOD对象，避免内存泄漏
{
    if (_sound != nullptr) _sound->release();
    if (_song != nullptr) _song->release();
    if (_system != nullptr) _system->release(); //关闭FMOD系统
}

void FModSoundProvider::PlaySound(std::string filename)
{
    ERRCHECK(_system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &_sound));
                    // ↑ 加载音频文件                // ↑ 默认模式，不循环
    ERRCHECK(_system->playSound(_sound, nullptr, false, &_soundChannel));
                    // ↑ 播放音效                       // ↑ 将播放通道存出其中； 
}

void FModSoundProvider::PlaySong(std::string filename, bool looping)
{
    FMOD_MODE mode = looping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
                             // ↑ 根据是否循环设置模式（支持循环播放BGM）
    ERRCHECK(_system->createSound(filename.c_str(), mode, 0, &_song));
    ERRCHECK(_system->playSound(_song, nullptr, false, &_musicChannel));
}

void FModSoundProvider::StopAllSounds()
{
    for (int i = 0; i < 100; ++i) //遍历通道，逐一停止
    {
        FMOD::Channel* channel = nullptr;
        if (_system->getChannel(i, &channel) == FMOD_OK && channel != nullptr)
        {
            channel->stop();  //强制停止所有有效通道
        }
    }
}
//播放状态查询
bool FModSoundProvider::IsSoundPlaying() //检查音效状态
{
    bool isPlaying = false;
    if (_soundChannel) //查询音效通道是否正在播放
        _soundChannel->isPlaying(&isPlaying);
    return isPlaying;
}

bool FModSoundProvider::IsSongPlaying() //检查音乐状态
{
    bool isPlaying = false;
    if (_musicChannel)
        _musicChannel->isPlaying(&isPlaying);
    return isPlaying;
}
//错误处理
void FModSoundProvider::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK) //统一检查FMOD函数返回值
    {
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}
//FMOD系统初始化
void FModSoundProvider::InitFMOD()
{
    FMOD_RESULT result;
    unsigned int version;
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    char name[256];
    // 1.创建FMOD系统对象
    ERRCHECK(FMOD::System_Create(&_system));
    ERRCHECK(_system->getVersion(&version));
    // 2.检查版本兼容性
    if (version < FMOD_VERSION)
    {
        std::cerr << "FMOD version mismatch!" << std::endl;
        exit(-1);
    }
    // 3.检测音频驱动
    ERRCHECK(_system->getNumDrivers(&numdrivers));
    if (numdrivers == 0)
    {
        ERRCHECK(_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
    }
    else
    {
        // 4.配置硬件参数（缓冲区大小、采样率etc.）
        ERRCHECK(_system->getDriverCaps(0, &caps, 0, &speakermode));
        ERRCHECK(_system->setSpeakerMode(speakermode));

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)
        {
            ERRCHECK(_system->setDSPBufferSize(1024, 10));
        }

        ERRCHECK(_system->getDriverInfo(0, name, 256, 0));
        if (strstr(name, "SigmaTel"))
        {
            ERRCHECK(_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR));
        }
    }
    // 5.初始化系统（失败时尝试降级到立体声模式）
    result = _system->init(100, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
        ERRCHECK(_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));
        ERRCHECK(_system->init(100, FMOD_INIT_NORMAL, 0));
    }
    else
    {
        ERRCHECK(result);
    }
}
//使用：
FModSoundProvider audio;
audio.PlaySound(".wav");
audio.PalySong("background.mp3",true);
if(audio.IsSongPlaying()) {...}
audio.StopAllSounds();

//Now 代码支持两个完全独立的音频系统
