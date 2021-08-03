//
//  SBBasePopup.hpp
//
//  Created by seongmin hwang on 2018. 5. 17..
//

#ifndef SBBasePopup_hpp
#define SBBasePopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../base/SBTypes.hpp"

class SBBasePopup : public cocos2d::Node, public SBNodeListener {
public:
    SBBasePopup();
    virtual ~SBBasePopup();
    
protected:
    virtual bool init() override;
    
    virtual void initBackgroundView();
    virtual void initContentView();
    
    virtual void addBackgroundChild(cocos2d::Node *child,
                                    int localZOrder = 0, int tag = cocos2d::Node::INVALID_TAG);
    virtual void addContentChild(cocos2d::Node *child,
                                 int localZOrder = 0, int tag = cocos2d::Node::INVALID_TAG);
    
public:
    virtual void dismiss();
    
    virtual void runBackgroundFadeAction(SBCallback onCompleted,
                                         float duration, float from, float to);
    virtual void runBackgroundFadeInAction(SBCallback onCompleted, float duration);
    virtual void runBackgroundFadeOutAction(SBCallback onCompleted, float duration);
    
protected:
    CC_SYNTHESIZE(cocos2d::ui::Layout*, backgroundView, BackgroundView);
    CC_SYNTHESIZE(cocos2d::Node*, contentView, ContentView);
    
    CC_SYNTHESIZE(SBCallbackNode, onDismissListener, OnDismissListener);
};

#endif /* SBBasePopup_hpp */
