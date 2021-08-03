//
//  PopupManager.hpp
//
//  Created by seongmin hwang on 2018. 7. 6..
//

#ifndef PopupManager_hpp
#define PopupManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "BasePopup.hpp"
#include "PopupDefine.h"
#include "PopupListener.hpp"

class PopupManager {
public:
    static PopupManager* getInstance();
    static void destroyInstance();
    ~PopupManager();
    
    static std::string getPopupInfo();
    
private:
    PopupManager();
    
public:
    void addPopup(BasePopup *popup);
    void removePopup(BasePopup *popup);
    
    static BasePopup*       getPopup(PopupType type);
    static BasePopup*       getFrontPopup();
    static size_t           getPopupCount();
    static size_t           getPopupCount(PopupType type);
    static bool             exists(PopupType type);
    
public:
    static BasePopup*       show(OnPopupEvent onEventListener, PopupType type, int zOrder = -1);
    static BasePopup*       show(PopupType type, int zOrder = -1);
    
    static void             cross(OnPopupEvent onEventListener,
                                  BasePopup *popup1, BasePopup *popup2,
                                  BasePopup *popup3 = nullptr);
    static void             cross(BasePopup *popup1, BasePopup *popup2,
                                  BasePopup *popup3 = nullptr);
    
    static BasePopup*       createPopup(PopupType type);
    
public:
    void addListener(PopupListener *listener);
    void addListener(cocos2d::Node *target, PopupListener *listener);
    
    void removeListener(PopupListener *listener);
    void removeListener(cocos2d::Ref *target);
    
    void dispatchEvent(BasePopup *popup, PopupEventType eventType);
    
private:
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<BasePopup*>, popups, Popups);
    cocos2d::Vector<PopupListener*> listeners;
};

#endif /* PopupManager_hpp */
