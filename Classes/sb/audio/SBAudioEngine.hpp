//
//  SBAudioEngine.hpp
//
//  Created by seongmin hwang on 2018. 3. 13..
//

#ifndef SBAudioEngine_hpp
#define SBAudioEngine_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

#include "../base/SBTypes.hpp"

class SBAudioEngine : public cocos2d::experimental::AudioEngine {
public:
    static SBAudioEngine* getInstance();
    static void destroyInstance();
    ~SBAudioEngine();
    
private:
    SBAudioEngine();
    
public:
    void init();
    
    void setMute(bool mute);
    void setEffectMute(bool mute);
    void setBGMMute(bool mute);
    
public:
    static int play2d(const std::string &file, bool loop = false, float volume = 1);
    
    static void resumeAll();
    static void resume(const std::string &file);
    
    static void pauseAll();
    static void pause(const std::string &file);
    
    static void stop(const std::string &file);
    
    static void setOnFinishedListener(int audioId,
                                      const std::function<void(int, const std::string&)> &listener);
    
    static float getDuration(int audioId);
    static float getDuration(const std::string &file);
    
// Effect
public:
    static int playEffect(const std::string &file, bool loop = false);
    
// BGM
public:
    static int playBGM(const std::string &bgm, bool loop = true);
    
    static void resumeBGM();
    static void pauseBGM();
    
    static void stopBGM();

private:
    static std::list<int> getAudioId(const std::string &file);
    
    static float getVolume(bool isMute);
    static float getEffectVolume();
    static float getBGMVolume();
    
private:
    SB_SYNTHESIZE_READONLY_BOOL(effectMute, EffectMute);  // 효과음 음소거 여부
    SB_SYNTHESIZE_READONLY_BOOL(bgmMute, BGMMute);        // 배경음 음소거 여부
    SB_SYNTHESIZE_READONLY_BOOL(paused, Paused);          // 일시정지 여부
    
    std::vector<std::string> effectFiles;
    std::string bgm;
    
    std::map<std::string, float> durationMap;
};

#endif /* SBAudioEngine_hpp */


