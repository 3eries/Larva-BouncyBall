//
//  PopupListener.hpp
//
//  Created by seongmin hwang on 2018. 7. 15..
//

#ifndef PopupListener_hpp
#define PopupListener_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

enum class PopupEventType {
    ENTER = 0,                  // 팝업 등장
    EXIT,                       // 팝업 퇴장
    ENTER_ACTION,               // 팝업 등장 연출 시작
    EXIT_ACTION,                // 팝업 퇴장 연출 시작
    ENTER_ACTION_FINISHED,      // 팝업 등장 연출 완료
    EXIT_ACTION_FINISHED,       // 팝업 퇴장 연출 완료
};

typedef std::function<void(/*BasePopup*/cocos2d::Node*, PopupEventType)> OnPopupEvent;

class PopupListener : public cocos2d::Ref {
public:
    SB_REF_CREATE_FUNC(PopupListener);
    
protected:
    PopupListener();
    virtual ~PopupListener();
    
    CC_SYNTHESIZE(cocos2d::Ref*, target, Target);
    OnPopupEvent onEvent;
};

#endif /* PopupListener_hpp */
