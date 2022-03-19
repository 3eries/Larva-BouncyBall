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
    Member("lobby_icon_w0n.png", "W0N",       "DESIGN"),
    Member("lobby_icon_b30.png", "B3O",       "ART"),
    Member("lobby_icon_kon.png", "KON",       "ART"),
    Member("lobby_icon_mw9.png", "GMACHINEE", "PROGRAMMING"),
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
    
    // Title
    auto title = Sprite::create(DIR_IMG_MAIN + "lobby_3eries_logo.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2TL(438, -198));
    addContentChild(title);
    
    // Peace
    auto peace = Sprite::create(DIR_IMG_MAIN + "lobby_text_peace.png");
    peace->setAnchorPoint(ANCHOR_M);
    peace->setPosition(Vec2TR(-406, -180));
    addContentChild(peace);
    
    // Member
    auto members = MEMBERS;
    
    std::random_device rd;
    std::mt19937 engine(rd());
    shuffle(members.begin(), members.end(), engine);
    
    Vec2 iconPos[] = {
        Vec2ML(182, 128),
        Vec2ML(182, -34),
        Vec2ML(182, -200),
        Vec2ML(182, -362),
    };
    
    Vec2 jobPos[] = {
        Vec2ML(266, 156),
        Vec2ML(266, -6),
        Vec2ML(266, -170),
        Vec2ML(266, -334),
    };
    
    Vec2 namePos[] = {
        Vec2ML(266, 98),
        Vec2ML(266, -64),
        Vec2ML(266, -228),
        Vec2ML(266, -392),
    };
    
    Vec2 temp(0,0);
    
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
        
        auto job = Label::createWithTTF(member.job, FONT_SUPER_STAR, 76, Size::ZERO,
                                        TextHAlignment::LEFT, TextVAlignment::CENTER);
        job->setTextColor(Color4B(255,211,0,255));
        // job->enableOutline(Color4B(0,0,0,255), 4);
        job->setAnchorPoint(ANCHOR_ML);
        job->setPosition(jobPos[i] + temp + Vec2(0, -2));
        addContentChild(job);
        
        auto name = Label::createWithTTF(member.name, FONT_SUPER_STAR, 76, Size::ZERO,
                                         TextHAlignment::LEFT, TextVAlignment::CENTER);
        name->setTextColor(Color4B(255,255,255,255));
        // name->enableOutline(Color4B(0,0,0,255), 4);
        name->setAnchorPoint(ANCHOR_ML);
        name->setPosition(Vec2(job->getPosition().x, namePos[i].y) + temp + Vec2(0, -2));
        addContentChild(name);
    }
}

