//
//  CommonLoadingBar.hpp
//
//  Created by seongmin hwang on 2018. 3. 29..
//

#ifndef CommonLoadingBar_hpp
#define CommonLoadingBar_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

class CommonLoadingBar : public BasePopup {
public:
    CREATE_FUNC(CommonLoadingBar);
    ~CommonLoadingBar();
    
private:
    CommonLoadingBar();
    
    bool init() override;
    void onEnter() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runEnterAction(float duratoin = 0, SBCallback onFinished = nullptr) override;
    void runExitAction(float duratoin = 0, SBCallback onFinished = nullptr) override;
    
public:
    CommonLoadingBar* show(cocos2d::Node *parent = nullptr, int zOrder = INT_MAX);
    CommonLoadingBar* show(int zOrder);
    CommonLoadingBar* dismissWithDelay(float delay);
    
    CommonLoadingBar* setMessage(const std::string &message);
    
private:
    bool isShowing;
    CC_SYNTHESIZE(float, uiDelay, UIDelay);
    
    int dotCount;
    
    CC_SYNTHESIZE_READONLY(std::string, message, Message);
};

#endif /* CommonLoadingBar_hpp */
