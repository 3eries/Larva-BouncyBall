//
//  GameUIHelper.hpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#ifndef GAMEGameUIHelper_h
#define GAMEGameUIHelper_h

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

// 버튼 정의
enum class ButtonType {
    FONT,           // 기본 폰트
};

struct ButtonSize {
    static const cocos2d::Size SMALL;
    static const cocos2d::Size MEDIUM;
    static const cocos2d::Size LARGE;
};

/** @class GameUIHelper
 * @brief UI 관련 공통 기능을 정의합니다
 */
class GameUIHelper {
public:
    static SBButton* createButton(SBButton::Config config);
    static SBButton* createFontButton(const std::string &title, const cocos2d::Size &size);
    
    static cocos2d::Sprite* createVIPMark(cocos2d::Node *removeAdsBtn);
    
    static cocos2d::Node* createThumbnailBorder(const cocos2d::Size &originSize,
                                                int whiteBorder, int blackBorder);
};

#endif /* GAMEGameUIHelper_h */
