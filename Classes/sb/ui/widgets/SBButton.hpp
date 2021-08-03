//
//  SBButton.hpp
//
//  Created by seongmin hwang on 2018. 2. 21..
//

#ifndef SBButton_hpp
#define SBButton_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../../base/SBTypes.hpp"
#include "../../base/SBMacros.h"

/** @class SBButton
 * @brief 기본 버튼
 * normal 상태의 파일명으로 버튼을 생성
 * 타이틀, 서브 타이틀 텍스트를 추가할 수 있음
 */
class SBButton : public cocos2d::ui::Widget {
public:
    typedef struct _Config {
        std::string file;
        cocos2d::Size size;
        std::string title;
        std::string font;
        float fontSize;
        
        _Config() {
            file = "";
            size = cocos2d::Size::ZERO;
            title = "";
            font = "";
            fontSize = 0;
        }
        
        _Config(const std::string &_file, const cocos2d::Size &_size,
                const std::string &_title,
                const std::string &_font, float _fontSize) {
            file = _file;
            size = _size;
            title = _title;
            font = _font;
            fontSize = _fontSize;
        }
    } Config;
    
public:
    static SBButton* create(const Config &config);
    static SBButton* create(const std::string &file,
                            const cocos2d::Size &size = cocos2d::Size::ZERO);
    static SBButton* create(const std::string &title, const std::string &font, float fontSize);
    virtual ~SBButton();
    
protected:
    SBButton();
    
    virtual bool init(const Config &config);
    virtual void initContents();
    virtual void initListener();
    
    virtual void onSizeChanged() override;
    virtual void updateTitlePosition();
    
    virtual void onTouchBegan();
    virtual void onTouchMoved();
    virtual void onTouchEnded();
    virtual void onTouchCanceled();
    virtual void onClick();
    
public:
    virtual void setScale9Enabled(bool scale9Enabled);
    virtual void setRenderingType(cocos2d::ui::Scale9Sprite::RenderingType type);
    
    virtual cocos2d::Label* setTitle(const std::string &title, int fontSize = 0);
    
// getter
public:
    inline std::string getTitleText()       { return titleLabel->getString(); }
    
protected:
    Config config;
    CC_SYNTHESIZE(SBCallbackNode, onClickListener, OnClickListener);
    CC_SYNTHESIZE(std::function<void(cocos2d::Node*, SBTouchEventType)>, onTouchListener, OnTouchListener);
    CC_SYNTHESIZE(float, zoomScale, ZoomScale);
    SB_SYNTHESIZE_READONLY_BOOL(scale9Enabled, Scale9Enabled);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Layout*, contentsLayer, ContentsLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, image, Image);
    CC_SYNTHESIZE_READONLY(cocos2d::Label*, titleLabel, Title);
};

#endif /* SBButton_hpp */
