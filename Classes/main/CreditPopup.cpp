//
//  CreditPopup.cpp
//
//  Created by seongmin hwang on 2018. 6. 28..
//

#include "CreditPopup.hpp"

#include "Define.h"
#include "GameUIHelper.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

// Member
struct Member {
    string image;
    string name;
    string job;
    
    Member(string _image, string _name, string _job) :
    image(_image), name(_name), job(_job) {}
};

static const vector<Member> MEMBERS = {
    Member("main_character_01.png", "GMACHINEE", "PROGRAMMING"),
    Member("main_character_02.png", "W0N",       "DESIGN"),
    Member("main_character_03.png", "B3O",       "ART"),
//    Member("main_character_04.png", "naconeee",  "TRANSLATION"),
//    Member("main_character_05.png", "JUNN",      "PROMOTIONAL VIDEO"),
};

CreditPopup::CreditPopup() : BasePopup(PopupType::CREDIT) {
}

CreditPopup::~CreditPopup() {
}

bool CreditPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    initCredit();
    
    return true;
}

void CreditPopup::onEnter() {
    
    BasePopup::onEnter();
}

/**
 * 등장 연출
 */
void CreditPopup::runEnterAction(SBCallback onFinished) {
    
    const float DURATION = 0.3f;
    
    BasePopup::runEnterAction(DURATION, onFinished);
    
    SBNodeUtils::recursiveCascadeOpacityEnabled(this, true);
    
    // fade in
    setOpacity(0);
    
    auto fadeIn = FadeIn::create(DURATION);
    auto callFunc = CallFunc::create([=]() {
        
        this->retain();
        
        if( onFinished ) {
            onFinished();
        }
        
        this->onEnterActionFinished();
        this->release();
    });
    runAction(Sequence::create(fadeIn, callFunc, nullptr));
}

/**
 * 등장 연출 완료
 */
void CreditPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
    
    // 화면 터치 시 팝업 종료
    auto touchNode = SBNodeUtils::createTouchNode();
    addChild(touchNode, SBZOrder::TOP);
    
    touchNode->addClickEventListener([=](Ref*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        this->dismiss();
    });
}

void CreditPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color4B(0,0,0,255*0.9f));
}

void CreditPopup::initCredit() {
    
    // 배너 영역을 고려하여 컨텐트뷰를 높인다
    // getContentView()->setPosition(getContentView()->getPosition() + Vec2(0,60));
    
    // Title
    auto title = Sprite::create(DIR_IMG_MAIN + "main_title_3eries.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2TC(0, -211));
    addContentChild(title);
    
    // Member
    auto members = MEMBERS;
    
    std::random_device rd;
    std::mt19937 engine(rd());
    shuffle(members.begin(), members.end(), engine);
    
    Vec2 iconPos[] = {
        Vec2MC(-246, 237),
        Vec2MC(-246, 65),
        Vec2MC(-246, -106),
        Vec2MC(-246, -278),
        Vec2MC(-246, -449),
    };
    
    Vec2 jobPos[] = {
        Vec2MC(18, 262),
        Vec2MC(18, 90),
        Vec2MC(18, -80),
        Vec2MC(18, -252),
        Vec2MC(18, -423),
    };
    
    Vec2 namePos[] = {
        Vec2MC(10, 209),
        Vec2MC(10, 37),
        Vec2MC(10, -133),
        Vec2MC(10, -305),
        Vec2MC(10, -476),
    };
    
    Vec2 temp(0,-100);
    
    for( int i = 0; i < MEMBERS.size(); ++i ) {
        auto member = members[i];
        
        auto icon = Sprite::create(DIR_IMG_MAIN + member.image);
        icon->setAnchorPoint(ANCHOR_M);
        icon->setPosition(iconPos[i] + temp);
        addContentChild(icon);
        
        // 아이콘 연출
        {
            float offset = 5 * (arc4random() % 2 == 0 ? 1 : -1);
            
            auto rotate1 = RotateTo::create(0.4f, offset);
            auto rotate2 = RotateTo::create(0.4f, -offset);
            auto seq = Sequence::create(rotate1, rotate2, nullptr);
            icon->runAction(RepeatForever::create(seq));
        }
        
        auto job = Label::createWithTTF(member.job, FONT_COMMODORE, 45, Size(485, 60),
                                        TextHAlignment::LEFT, TextVAlignment::CENTER);
        job->setTextColor(Color4B(255,211,0,255));
        job->enableOutline(Color4B(0,0,0,255), 4);
        job->setOverflow(Label::Overflow::SHRINK);
        job->setAnchorPoint(ANCHOR_ML);
        job->setPosition(jobPos[i] + Vec2(-355*0.5f, 4) + temp);
        addContentChild(job);
        
        auto name = Label::createWithTTF(member.name, FONT_RETRO, 45, job->getDimensions(),
                                         TextHAlignment::LEFT, TextVAlignment::CENTER);
        name->setTextColor(Color4B(255,255,255,255));
        name->enableOutline(Color4B(0,0,0,255), 4);
        name->setAnchorPoint(ANCHOR_ML);
        name->setPosition(Vec2(job->getPosition().x, namePos[i].y) + Vec2(0, 4) + temp);
        addContentChild(name);
    }
}

