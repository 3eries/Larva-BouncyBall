//
//  ShopPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#include "ShopPopup.hpp"

#include "User.hpp"
#include "GameUIHelper.hpp"

#include "IAPCell.hpp"
#include "CharacterCell.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace std;

// shop_column_deal.png Vec2MC(-676, -24) , Size(384, 656)
// shop_column_bg.png Vec2MC(-260, -24) , Size(384, 656)
// shop_column_bg.png Vec2MC(156, -24) , Size(384, 656)
// mask Vec2MC(0, -24) , Size(1736, 656)
////////////
// shop_column_deal.png Vec2MC(-676, -48) , Size(384, 736)
// shop_column_bg.png Vec2MC(-260, -48) , Size(384, 736)
// shop_column_bg.png Vec2MC(156, -48) , Size(384, 736)
// shop_column_bg.png Vec2MC(572, -48) , Size(384, 736)
// mask Vec2MC(0, -48) , Size(1736, 736)

#define CHARACTER_LIST_SIZE_1               Size(1736-384-32, 656)   // Cell 3개
#define CHARACTER_LIST_SIZE_2               Size(1736, 656)          // Cell 4개
#define CHARACTER_LIST_CELL_SIZE            Size(384, 656)
#define CHARACTER_LIST_CELL_PADDING         32

ShopPopup::ShopPopup(): BasePopup(PopupType::SHOP),
iapCell(nullptr),
characterListView(nullptr) {
}

ShopPopup::~ShopPopup() {
    
}

bool ShopPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    initCharacterList();
    updatePositionForBanner(nodeListForBanner);
    
    // 광고 제거 이벤트
    auto listener = EventListenerCustom::create(DIRECTOR_EVENT_REMOVE_ADS, [=](EventCustom *event) {
        this->initCharacterList();
        updatePositionForBanner(nodeListForBanner);
    });
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ShopPopup::onEnter() {
    
    BasePopup::onEnter();
    
    SBAnalytics::logEvent(ANALYTICS_EVENT_SHOP);
    
    runEnterAction();
}

bool ShopPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    
    // lazy remove
    // ShopPopup을 바로 삭제하면 다른 Node에 onBackKeyReleased가 호출됨
    // dismissWithAction();
    setVisible(false);
    
    SBDirector::postDelayed(this, [=]() {
        this->dismiss();
    }, 0.1f, true);

    return true;
}

void ShopPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color4B(0,0,0,255*0.9f));
}

void ShopPopup::initContentView() {
    
    BasePopup::initContentView();
    
    auto bg = Sprite::create(DIR_IMG_SHOP + "shop_bg.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0, 10));
    addContentChild(bg);
    
    nodeListForBanner.push_back(NodeAndPosition(bg));
    
    // 타이틀
    auto title = Sprite::create(DIR_IMG_SHOP + "shop_title_shop.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2TC(0, -128));
    addContentChild(title);
    
    nodeListForBanner.push_back(NodeAndPosition(title));
    
    // 닫기 버튼
    auto closeBtn = SBButton::create(DIR_IMG_SHOP + "shop_btn_close.png");
    closeBtn->setZoomScale(ButtonZoomScale::NORMAL);
    closeBtn->setAnchorPoint(ANCHOR_M);
    closeBtn->setPosition(Vec2TR(-74, -114));
    addContentChild(closeBtn);
    
    closeBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        this->dismissWithAction();
    });
}

void ShopPopup::initCharacterList() {
    
    if( iapCell )           iapCell->removeFromParent();
    if( characterListView ) characterListView->removeFromParent();
    
    // IAP Cell
    const bool ownIAPItem = User::isRemovedAds();
    
    auto characterListSize = CHARACTER_LIST_SIZE_2;
    Vec2 characterListPos;
    characterListPos.x = (SB_WIN_SIZE.width - CHARACTER_LIST_SIZE_2.width) / 2;
    characterListPos.y = SB_WIN_SIZE.height*0.5f - 48;
    
    if( !ownIAPItem ) {
        iapCell = IAPCell::create();
        iapCell->setAnchorPoint(ANCHOR_ML);
        iapCell->setPosition(characterListPos);
        addContentChild(iapCell);

        iapCell->setOnClickListener([=]() {
            this->onClickIAP();
        });
        
        characterListSize = CHARACTER_LIST_SIZE_1;
        characterListPos.x = SBNodeUtils::getBoundingBoxInWorld(iapCell).getMaxX() + CHARACTER_LIST_CELL_PADDING;
    }
    
    // ListView - CharacterList
    characterListView = ListView::create();
    characterListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    // characterListView->setPadding(50, 0, 0, 0);
    characterListView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    characterListView->setItemsMargin(CHARACTER_LIST_CELL_PADDING);
    characterListView->setBounceEnabled(true);
    characterListView->setScrollBarEnabled(false);
    characterListView->setAnchorPoint(ANCHOR_ML);
    characterListView->setPosition(WITH_BANNER_SIZE(characterListPos));
    characterListView->setContentSize(characterListSize);
    characterListView->addEventListener([=](Ref*, ScrollView::EventType eventType) {
      
        if( eventType == ScrollView::EventType::AUTOSCROLL_ENDED ) {
            // CCLOG("AUTOSCROLL_ENDED");
            // characterListView->setBounceEnabled(true);
        }
    });
    
    addContentChild(characterListView);
    
    auto characters = CHARACTER_MANAGER->getCharacters();
    auto characterOrder = CHARACTER_MANAGER->getCharacterOrder();
    
    for( int i = 0; i < characters.size() ; ++i ) {
        auto cell = CharacterCell::create(CHARACTER_MANAGER->getCharacter(characterOrder[i]));
        cell->setAnchorPoint(ANCHOR_M);
        characterListView->pushBackCustomItem(cell);
        
        cell->setOnSelectListener([=](CharacterCell *cell) {
            this->onCharacterSelect(characterListView, cell);
        });
    }
    
    // 리스트 영역 확인용
//    auto n = LayerColor::create(Color4B(255,0,0,255*0.3f));
//    n->setIgnoreAnchorPointForPosition(false);
//    n->setAnchorPoint(characterListView->getAnchorPoint());
//    n->setPosition(characterListView->getPosition());
//    n->setContentSize(characterListView->getContentSize());
//    addContentChild(n);
}

/**
 * 캐릭터 선택
 */
void ShopPopup::onCharacterSelect(ListView *listView, CharacterCell *cell) {
    
    CHARACTER_MANAGER->setSelected(cell->getData().charId);
    
    // 모든 Cell 선택 취소
    auto items = listView->getItems();
    
    for( auto item : items ) {
        dynamic_cast<CharacterCell*>(item)->unselect();
    }
    
    // 해당 Cell 선택
    cell->select();
    
    // 통계 이벤트
    {
        SBAnalytics::EventParams params;
        params[ANALYTICS_EVENT_PARAM_CHAR_ID] = SBAnalytics::EventParam(cell->getData().charId);

        SBAnalytics::logEvent(ANALYTICS_EVENT_CHARACTER_SELECT, params);
    }
}

/**
 * IAP 아이템 클릭
 */
void ShopPopup::onClickIAP() {
    
    SBDirector::getInstance()->setScreenTouchLocked(true);

    auto listener = iap::PurchaseListener::create();
    listener->setTarget(this);
    listener->onPurchased = [=](const iap::Item &item) {
        
        // 통계 이벤트
        {
            SBAnalytics::EventParams params;
            params[ANALYTICS_EVENT_PARAM_CLEARED_STAGE_COUNT] = SBAnalytics::EventParam(TO_STRING(StageManager::getClearedStageCount()));
            params[ANALYTICS_EVENT_PARAM_POPUP] = SBAnalytics::EventParam("shop");

            SBAnalytics::logEvent(ANALYTICS_EVENT_IAP_REMOVE_ADS, params);
        }
        
        // 광고 제거
        User::removeAds();
    };

    listener->onFinished = [=](bool result) {
        SBDirector::postDelayed(this, [=]() {
            SBDirector::getInstance()->setScreenTouchLocked(false);
        }, 0.2f);
    };

    iap::IAPHelper::purchaseRemoveAds(listener);
}
