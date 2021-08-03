//
//  SBButton.cpp
//
//  Created by seongmin hwang on 2018. 2. 21..
//

#include "SBButton.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

static const float ZOOM_SCALE               = 0.05f;

SBButton* SBButton::create(const Config &config) {
    
    SBButton *btn = new (nothrow)SBButton;
    
    if( btn && btn->init(config) ) {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return nullptr;
}

SBButton* SBButton::create(const string &file, const Size &size) {
    
    return create(Config(file, size, "", "", 0));
}

SBButton* SBButton::create(const string &title, const string &font, float fontSize) {
    
    return create(Config("", Size::ZERO, title, font, fontSize));
}

SBButton::SBButton() :
onClickListener(nullptr),
onTouchListener(nullptr),
zoomScale(ZOOM_SCALE),
scale9Enabled(false),
contentsLayer(nullptr),
image(nullptr),
titleLabel(nullptr) {
}

SBButton::~SBButton() {
    
}

bool SBButton::init(const Config &config) {
    
    if( !Widget::init() ) {
        return false;
    }
    
    this->config = config;
    
    // init
    setAnchorPoint(ANCHOR_M);
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    
    initContents();
    initListener();
    
    // apply size
    Size size = config.size;
    
    if( size.equals(Size::ZERO) && image ) {
        size = image->getContentSize();
    }
    
    if( getContentSize().equals(size) ) {
        onSizeChanged();
    } else {
        setContentSize(size);
    }
    
    // test
    /*
    auto n = LayerColor::create(Color4B(255,0,0,255*0.5f));
    n->setIgnoreAnchorPointForPosition(false);
    n->setAnchorPoint(Vec2::ZERO);
    n->setPosition(Vec2::ZERO);
    n->setContentSize(size);
    addChild(n, -1);
    */
    
    return true;
}

/**
 * 버튼 컨텐츠 초기화
 */
void SBButton::initContents() {
    
    contentsLayer = Layout::create();
    contentsLayer->setAnchorPoint(ANCHOR_M);
    contentsLayer->setPosition(Vec2::ZERO);
    contentsLayer->setCascadeColorEnabled(true);
    contentsLayer->setCascadeOpacityEnabled(true);
    addChild(contentsLayer);
    
    // image
    if( config.file != "" ) {
        image = ImageView::create(config.file);
        image->setScale9Enabled(true);
        image->setAnchorPoint(Vec2::ZERO);
        image->setPosition(Vec2::ZERO);
        contentsLayer->addChild(image);
        
        setRenderingType(Scale9Sprite::RenderingType::SIMPLE);
    }
    
    // label
    if( config.title != "" ) {
        if( config.font == "" ) {
            titleLabel = Label::createWithSystemFont(config.title, "", config.fontSize);
        } else {
            titleLabel = Label::createWithTTF(config.title, config.font, config.fontSize);
        }
        
        titleLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        titleLabel->setAnchorPoint(ANCHOR_M);
        titleLabel->setColor(Color3B::WHITE);
        contentsLayer->addChild(titleLabel);
    }
}

/**
 * 리스너 초기화
 */
void SBButton::initListener() {
    
    setTouchEnabled(true);
    
    // 터치 이벤트 등록
    addTouchEventListener([=](Ref *sender, Widget::TouchEventType eventType) {
        switch( eventType ) {
            case Widget::TouchEventType::BEGAN:         this->onTouchBegan();       break;
            case Widget::TouchEventType::MOVED:         this->onTouchMoved();       break;
            case Widget::TouchEventType::ENDED:         this->onTouchEnded();       break;
            case Widget::TouchEventType::CANCELED:      this->onTouchCanceled();    break;
            default: break;
        }
    });
    
    // 클릭 이벤트 등록
    addClickEventListener([=](Ref*) {
        this->onClick();
    });
}

void SBButton::onSizeChanged() {
    
    Widget::onSizeChanged();
    
    if( !contentsLayer ) {
        return;
    }
    
    Size size = getContentSize();
    contentsLayer->setPosition(Vec2MC(size, 0, 0));
    contentsLayer->setContentSize(size);
    
    if( image ) image->setContentSize(size);
    
    updateTitlePosition();
}

/**
 * 타이틀 좌표 변경
 */
void SBButton::updateTitlePosition() {
    
    if( !titleLabel ) {
        return;
    }
    
    Size size = getContentSize();
    titleLabel->setPosition(Vec2MC(size, 0, 0));
}

/**
 * 터치 시작
 */
void SBButton::onTouchBegan() {
    
    retain();
    
    if( onTouchListener ) {
        onTouchListener(this, SBTouchEventType::BEGAN);
    }
    
    contentsLayer->setScale(1 + zoomScale);
    
    release();
}

/**
 * 터치 이동
 */
void SBButton::onTouchMoved() {

    retain();
    
    if( onTouchListener ) {
        onTouchListener(this, SBTouchEventType::MOVED);
    }
    
    release();
}

/**
 * 터치 종료
 */
void SBButton::onTouchEnded() {
    
    retain();
    
    if( onTouchListener ) {
        onTouchListener(this, SBTouchEventType::ENDED);
    }
    
    contentsLayer->setScale(1);
    
    release();
}

/**
 * 터치 취소
 */
void SBButton::onTouchCanceled() {
    
    retain();
    
    if( onTouchListener ) {
        onTouchListener(this, SBTouchEventType::CANCELED);
    }
    
    contentsLayer->setScale(1);
    
    release();
}

/**
 * 클릭
 */
void SBButton::onClick() {
 
    SB_SAFE_PERFORM_LISTENER_N(this, onClickListener);
}

void SBButton::setScale9Enabled(bool scale9Enabled) {
    
    if( this->scale9Enabled == scale9Enabled ) {
        return;
    }
    
    this->scale9Enabled = scale9Enabled;
    setRenderingType(scale9Enabled ? Scale9Sprite::RenderingType::SLICE : Scale9Sprite::RenderingType::SIMPLE);
}

void SBButton::setRenderingType(Scale9Sprite::RenderingType type) {

    if( image ) {
        auto renderer = dynamic_cast<Scale9Sprite*>(image->getVirtualRenderer());
        renderer->setRenderingType(type);
    }
}

/**
 * 타이틀 설정
 */
Label* SBButton::setTitle(const string &title, int fontSize) {
    
    if( fontSize != 0 ) {
        if( this->config.font == "" ) {
            titleLabel->setSystemFontSize(fontSize);
        } else {
            auto ttfConfig = titleLabel->getTTFConfig();
            ttfConfig.fontSize = fontSize;
            titleLabel->setTTFConfig(ttfConfig);
        }
    }
    
    titleLabel->setString(title);
    
    return titleLabel;
}

