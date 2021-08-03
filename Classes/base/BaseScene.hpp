//
//  BaseScene.hpp
//
//  Created by seongmin hwang on 2018. 7. 19..
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "SceneManager.h"

class BaseScene : public cocos2d::Scene, public SBNodeListener {
protected:
    BaseScene();
    virtual ~BaseScene();
    
protected:
    virtual bool init() override;
    virtual void cleanup() override;
    
    virtual bool onApplicationEnterBackground() override { return true; }
    virtual bool onApplicationEnterForeground() override { return true; }
    virtual bool onBackKeyReleased() override;
    virtual void onClick(cocos2d::Node *sender) override {}
    
    virtual void setSceneTouchLocked(bool isLocekd);
    
    virtual void replaceScene(SceneType type);
    
protected:
    cocos2d::Node *touchLockNode;
};

#endif /* BaseScene_hpp */
