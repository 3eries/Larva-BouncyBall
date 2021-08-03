//
//  TutorialView.hpp
//
//  Created by seongmin hwang on 20/07/2019.
//

#ifndef TutorialView_hpp
#define TutorialView_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

struct TutorialConfig {
    cocos2d::Rect  focusBox;
    cocos2d::Node *focusNode;
    
    TutorialConfig() : focusNode(nullptr) {}
};

class TutorialView : public cocos2d::Node {
public:
    static TutorialView* create(const TutorialConfig &config = TutorialConfig());
    virtual ~TutorialView();
    
protected:
    TutorialView();
    
    bool init(const TutorialConfig &config);
    void initFocusBox();
    
protected:
    CC_SYNTHESIZE_READONLY(TutorialConfig, config, Config);
    CC_SYNTHESIZE(SBCallbackNode, onFocusBoxTouchListener, OnFocusBoxTouchListener);
    CC_SYNTHESIZE(SBCallbackNode, onTouchListener, OnTouchListener);
};

#endif /* TutorialView_hpp */
