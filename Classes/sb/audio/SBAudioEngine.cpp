//
//  SBAudioEngine.cpp
//
//  Created by seongmin hwang on 2018. 3. 13..
//

#include "SBAudioEngine.hpp"

#include "../base/SBUserDefaultKey.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace std;

static SBAudioEngine *instance = nullptr;
SBAudioEngine* SBAudioEngine::getInstance() {
    
    if( !instance ) {
        instance = new SBAudioEngine();
    }
    
    return instance;
}

void SBAudioEngine::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

SBAudioEngine::SBAudioEngine() :
effectMute(false),
bgmMute(false),
paused(false),
bgm("") {
}

SBAudioEngine::~SBAudioEngine() {
}

/**
 * 매니저 초기화
 */
void SBAudioEngine::init() {
    
    setEffectMute(UserDefault::getInstance()->getBoolForKey(SBUserDefaultKey::EFFECT_MUTE, false));
    setBGMMute(UserDefault::getInstance()->getBoolForKey(SBUserDefaultKey::BGM_MUTE, false));
}

/**
 * mute 설정
 */
void SBAudioEngine::setMute(bool mute) {
    
    setEffectMute(mute);
    setBGMMute(mute);
    
    // update audio engine
    /*
    const float VOLUME = getVolume();
    
    for( auto it = _audioIDInfoMap.begin(); it != _audioIDInfoMap.end(); ++it ) {
        AudioEngine::setVolume(it->first, VOLUME);
//        _audioEngineImpl->setVolume((int)it->first, VOLUME);
//        it->second.volume = VOLUME;
    }
     */
}

/**
 * 효과음 음소거 설정
 */
void SBAudioEngine::setEffectMute(bool mute) {
    
    this->effectMute = mute;
    
    // update user default
    UserDefault::getInstance()->setBoolForKey(SBUserDefaultKey::EFFECT_MUTE, mute);
    UserDefault::getInstance()->flush();
    
    // update audio engine
    const float VOLUME = getEffectVolume();
    
    for( string file : effectFiles ) {
        auto audioIds = getAudioId(file);
        
        for( int audioId : audioIds ) {
            AudioEngine::setVolume(audioId, VOLUME);
        }
    }
}

/**
 * 배경음 음소거 설정
 */
void SBAudioEngine::setBGMMute(bool mute) {
    
    this->bgmMute = mute;
    
    // update user default
    UserDefault::getInstance()->setBoolForKey(SBUserDefaultKey::BGM_MUTE, mute);
    UserDefault::getInstance()->flush();
    
    // update audio engine
    const float VOLUME = getBGMVolume();
    auto audioIds = getAudioId(bgm);
    
    for( int audioId : audioIds ) {
        AudioEngine::setVolume(audioId, VOLUME);
    }
}

/**
 * play
 */
int SBAudioEngine::play2d(const string &file, bool loop, float volume) {
    
    if( file == "" ) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
    auto audioId = AudioEngine::play2d(file, loop, volume);
    
    if( instance->paused ) {
        // TODO:
    }
    
    return audioId;
}

/**
 * resume
 */
void SBAudioEngine::resumeAll() {
    
    instance->paused = false;
    AudioEngine::resumeAll();
}

void SBAudioEngine::resume(const string &file) {
    
    auto audioIds = getAudioId(file);
    
    for( int audioId : audioIds ) {
        AudioEngine::resume(audioId);
    }
}

/**
 * pause
 */
void SBAudioEngine::pauseAll() {
    
    instance->paused = true;
    AudioEngine::pauseAll();
}

void SBAudioEngine::pause(const string &file) {
    
    auto audioIds = getAudioId(file);
    
    for( int audioId : audioIds ) {
        AudioEngine::pause(audioId);
    }
}

/**
 * stop
 */
void SBAudioEngine::stop(const string &file) {
    
    auto audioIds = getAudioId(file);
    
    for( int audioId : audioIds ) {
        AudioEngine::stop(audioId);
    }
}

void SBAudioEngine::setOnFinishedListener(int audioId, const function<void(int, const string&)> &listener) {
    
    AudioEngine::setFinishCallback(audioId, listener);
}

float SBAudioEngine::getDuration(int audioId) {
    
    float duration = AudioEngine::getDuration(audioId);
    return duration;
}

float SBAudioEngine::getDuration(const string &file) {
    
    auto audioIds = getAudioId(file);
    if( audioIds.size() == 0 ) {
        return 0;
    }
    
    float duration = AudioEngine::getDuration(*audioIds.begin());
    instance->durationMap[file] = duration;
    
    return duration;
}

/**
 * 효과음 재생
 */
int SBAudioEngine::playEffect(const string &file, bool loop) {
    
    if( file == "" ) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
    // 효과음 파일 추가
    auto effectFiles = instance->effectFiles;
    bool isFind = false;
    
    for( string effectFile : effectFiles ) {
        if( effectFile == file ) {
            isFind = true;
            break;
        }
    }
    
    if( !isFind ) {
        effectFiles.push_back(file);
    }
    
    // 재생
    return SBAudioEngine::play2d(file, loop, getEffectVolume());
}

/**
 * 배경음 재생
 */
int SBAudioEngine::playBGM(const string &bgm, bool loop) {
    
    if( bgm == "" ) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
    if( instance->bgm == bgm ) {
        return *getAudioId(bgm).begin();
    }
    
    // 이전 BGM 정지
    /*
    string runningBgm = instance->bgm;
    
    if( runningBgm != "" ) {
        AudioEngine::uncache(runningBgm);
    }
    */
    
    stopBGM();
    
    // 새로운 BGM 재생
    instance->bgm = bgm;
    return SBAudioEngine::play2d(bgm, loop, getBGMVolume());
}

/**
 * 배경음 재개
 */
void SBAudioEngine::resumeBGM() {
    
    if( instance->bgm != "" ) {
        resume(instance->bgm);
    }
}

/**
 * 배경음 일시정지
 */
void SBAudioEngine::pauseBGM() {
    
    if( instance->bgm != "" ) {
        pause(instance->bgm);
    }
}

/**
 * 배경음 정지
 */
void SBAudioEngine::stopBGM() {
    
    if( instance->bgm != "" ) {
        stop(instance->bgm);
        instance->bgm = "";
    }
}

list<int> SBAudioEngine::getAudioId(const string &file) {
    
    auto it = _audioPathIDMap.find(file);
    if( it != _audioPathIDMap.end() ) {
        return it->second;
    }
    
    return list<int>();
}

float SBAudioEngine::getVolume(bool isMute) {
    return isMute ? 0 : 1;
}

float SBAudioEngine::getEffectVolume() {
    return getVolume(instance->effectMute);
}

float SBAudioEngine::getBGMVolume() {
    return getVolume(instance->bgmMute);
}


