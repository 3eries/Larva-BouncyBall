//
//  SceneManager.hpp
//
//  Created by seongmin hwang on 2018. 2. 22..
//

#ifndef SceneManager_h
#define SceneManager_h

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

enum class SceneType {
    NONE,
    TEST,
    SPLASH,
    MAIN,
    GAME,
};

static inline std::string getSceneName(SceneType type) {
    switch( type ) {
        case SceneType::TEST:            return "TEST";
        case SceneType::SPLASH:          return "SPLASH";
        case SceneType::MAIN:            return "MAIN";
        case SceneType::GAME:            return "GAME";
        default:
            return "NOT DEFINED";
    }
}

/** @class SceneManager
 * @brief 이 클래스는 공통 데이터를 관리하고 Scene 전환을 담당합니다 
 */
class SceneManager : public cocos2d::Ref {
public:
    static const float REPLACE_DURATION_SPLASH_TO_MAIN;
    static const float REPLACE_DURATION_MAIN;
    static const float REPLACE_DURATION_GAME;
    
public:
    static SceneManager* getInstance();
    static void destroyInstance();
    ~SceneManager();
    
private:
    SceneManager();
    
    cocos2d::Scene* createScene(SceneType type);
    
public:
    void replace(SceneType type, std::function<cocos2d::Scene*()> createSceneFunc);
    void replace(SceneType type);
    
    static bool onBackKeyReleased();
    static void setBackKeyLocked(bool isLocked);
    
    // Getter
    static SceneType          getSceneType();
    static SceneType          getPreviousSceneType();
    static cocos2d::Scene*    getScene();
    static bool               isGameScene();
    
private:
    SceneType sceneType;
    SceneType prevSceneType;
    cocos2d::Scene *scene;
    
    bool isRunningReplaceScene;
    bool isBackKeyLocked;
};

#endif /* SceneManager_h */
