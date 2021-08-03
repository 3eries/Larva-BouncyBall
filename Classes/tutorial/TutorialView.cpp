//
//  TutorialView.cpp
//
//  Created by seongmin hwang on 20/07/2019.
//

#include "TutorialView.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

TutorialView* TutorialView::create(const TutorialConfig &config) {
    
    auto view = new TutorialView();
    
    if( view && view->init(config) ) {
        view->autorelease();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

TutorialView::TutorialView() :
onFocusBoxTouchListener(nullptr),
onTouchListener(nullptr) {
}

TutorialView::~TutorialView() {
}

bool TutorialView::init(const TutorialConfig &config) {
    
    if( !Node::init() ) {
        return false;
    }
    
    this->config = config;

    setAnchorPoint(ANCHOR_M);
    setPosition(Vec2MC(0,0));
    setContentSize(SB_WIN_SIZE);
    
    initFocusBox();
    
    return true;
}

void TutorialView::initFocusBox() {
    
    if( config.focusBox.equals(Rect::ZERO) && config.focusNode ) {
        config.focusBox = SB_BOUNDING_BOX_IN_WORLD(config.focusNode);
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event*) -> bool {
        
        if( !SBNodeUtils::hasVisibleParents(this) ) {
            return false;
        }
        
        // 포커스 영역에 터치 이벤트 넘겨줍니다
        if( !config.focusBox.equals(Rect::ZERO) &&
            config.focusBox.containsPoint(touch->getLocation()) ) {
            if( onFocusBoxTouchListener ) {
                onFocusBoxTouchListener(this);
            }
            return false;
        }
        
        if( onTouchListener ) {
            onTouchListener(this);
        }
        
        return true;
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
