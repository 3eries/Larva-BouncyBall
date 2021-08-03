//
//  BasePopup.hpp
//
//  Created by seongmin hwang on 2018. 7. 5..
//

#ifndef BasePopup_hpp
#define BasePopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "PopupDefine.h"
#include "PopupListener.hpp"

class PopupManager;

class BasePopup : public SBBasePopup {
public:
    static BasePopup* create();
    virtual ~BasePopup();
    
protected:
    BasePopup(PopupType type);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void cleanup() override;
    virtual bool onBackKeyReleased() override;
    
    virtual void onPopupEvent(PopupEventType eventType);
    
public:
    virtual void setBackgroundColor(const cocos2d::Color4B &color);
    
    virtual void dismissWithAction(SBCallback onFinished = nullptr);
    
    virtual void runEnterAction(float duration, SBCallback onFinished = nullptr);
    virtual void runEnterAction(SBCallback onFinished = nullptr);
    
    virtual void runExitAction(float duration, SBCallback onFinished = nullptr);
    virtual void runExitAction(SBCallback onFinished = nullptr);
    
    virtual void onEnterActionFinished();
    virtual void onExitActionFinished();
    
    virtual void runSlideAction(SBCallback onFinished,
                                float duratoin, cocos2d::Vec2 from, cocos2d::Vec2 to);
    virtual void runSlideInAction(SBCallback onFinished, float duratoin);
    virtual void runSlideOutAction(SBCallback onFinished, float duratoin);
    
protected:
    PopupManager *popupMgr;
    CC_SYNTHESIZE_READONLY(PopupType, type, Type);
    CC_SYNTHESIZE(OnPopupEvent, onPopupEventListener, OnPopupEventListener);
    CC_SYNTHESIZE(SBCallbackNode, onBackKeyReleasedListener, OnBackKeyReleasedListener);
    
    CC_SYNTHESIZE(float, enterTimeScale, EnterTimeScale);       // 등장 연출 타임 스케일
    CC_SYNTHESIZE(float, exitTimeScale,  ExitTimeScale);        // 퇴장 연출 타임 스케일
    
    SB_SYNTHESIZE_BOOL(runningEnterAction, RunningEnterAction); // 등장 연출 진행 여부
    SB_SYNTHESIZE_BOOL(runningExitAction, RunningExitAction);   // 퇴장 연출 진행 여부
    
    // 팝업 제거 시 퇴장 연출 무시 여부
    SB_SYNTHESIZE_BOOL(ignoreDismissAction, IgnoreDismissAction);
};

#endif /* BasePopup_hpp */
