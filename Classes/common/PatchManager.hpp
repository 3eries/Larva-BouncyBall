//
//  PatchManager.hpp
//
//  Created by seongmin hwang on 2018. 6. 6..
//

#ifndef PatchManager_hpp
#define PatchManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

struct AppVersion {
    std::string latest;           // 최신 버전
    std::string forceUpdate;      // 강제 업데이트 버전
};

class PatchManager {
public:
    static PatchManager* getInstance();
    static void destroyInstance();
    ~PatchManager();
    
private:
    PatchManager();
    
public:
    void init();
    
    void patch();
    
    void        setExternalConfigFileVersion(std::string version);
    std::string getExternalConfigFileVersion();
    
private:
    void copyConfigFile();

private:
    CC_SYNTHESIZE(std::function<void(AppVersion)>,
                  onAppVersion, OnAppVersion);
    CC_SYNTHESIZE(std::function<void(std::string/*version*/, std::string/*json*/)>,
                  onGameConfig, OnGameConfig);
};

#endif /* PatchManager_hpp */
