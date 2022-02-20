//
//  ShopPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#include "ShopPopup.hpp"

#include "Define.h"
#include "User.hpp"
#include "GameUIHelper.hpp"

#include "IAPCell.hpp"
#include "CharacterCell.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

// shop_column_deal.png Vec2MC(-676, -48) , Size(384, 736)
// shop_column_bg.png Vec2MC(-260, -48) , Size(384, 736)
// shop_column_bg.png Vec2MC(156, -48) , Size(384, 736)
// shop_column_bg.png Vec2MC(572, -48) , Size(384, 736)
// mask Vec2MC(0, -48) , Size(1736, 736)

#define CHARACTER_LIST_SIZE_1               Size(1736-384-32, 736)   // Cell 3개
#define CHARACTER_LIST_SIZE_2               Size(1736, 736)          // Cell 4개
#define CHARACTER_LIST_CELL_SIZE            Size(384, 736)
#define CHARACTER_LIST_CELL_PADDING         32

ShopPopup::ShopPopup(): BasePopup(PopupType::SHOP) {
}

ShopPopup::~ShopPopup() {
    
}

bool ShopPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ShopPopup::onEnter() {
    
    BasePopup::onEnter();
    
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
    }, 0.1f);

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
    bg->setPosition(Vec2MC(0, -24));
    addContentChild(bg);
    
    // 타이틀
    auto title = Sprite::create(DIR_IMG_SHOP + "shop_title_shop.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2TC(2, -128));
    addContentChild(title);
    
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
    
    // IAP Cell
    const bool ownIAPItem = User::isRemovedAds();
    
    auto characterListSize = CHARACTER_LIST_SIZE_2;
    Vec2 characterListPos;
    characterListPos.x = (SB_WIN_SIZE.width - CHARACTER_LIST_SIZE_2.width) / 2;
    characterListPos.y = SB_WIN_SIZE.height*0.5f - 48;
    
    if( !ownIAPItem ) {
        // 가운데 정렬을 위한 좌표 계산
        // float firstCellX = (SB_WIN_SIZE.width - CHARACTER_LIST_SIZE_2.width) / 2;
        
        auto iapCell = IAPCell::create();
        iapCell->setAnchorPoint(ANCHOR_ML);
        iapCell->setPosition(characterListPos);
        addContentChild(iapCell);

        iapCell->setOnClickListener([=]() {
            // TODO: IAP
            CCLOG("상품 구매");
    //        SBDirector::getInstance()->setScreenTouchLocked(true);
    //
    //        auto listener = iap::PurchaseListener::create();
    //        listener->setTarget(this);
    //        listener->onPurchased = [=](const iap::Item &item) {
    //            User::removeAds();
    //            this->dismissWithAction();
    //        };
    //
    //        listener->onFinished = [=](bool result) {
    //            SBDirector::postDelayed(this, [=]() {
    //                SBDirector::getInstance()->setScreenTouchLocked(false);
    //            }, 0.2f);
    //        };
    //
    //        iap::IAPHelper::purchaseRemoveAds(listener);
        });
        
        characterListSize = CHARACTER_LIST_SIZE_1;
        characterListPos.x = SBNodeUtils::getBoundingBoxInWorld(iapCell).getMaxX() + CHARACTER_LIST_CELL_PADDING;
    }
    
    // ScrollView - CharacterList
    auto characterListView = ListView::create();
    characterListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    // characterListView->setPadding(50, 0, 0, 0);
    characterListView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    characterListView->setItemsMargin(CHARACTER_LIST_CELL_PADDING);
    characterListView->setBounceEnabled(true);
    characterListView->setScrollBarEnabled(false);
    characterListView->setAnchorPoint(ANCHOR_ML);
    characterListView->setPosition(characterListPos);
    characterListView->setContentSize(CHARACTER_LIST_SIZE_1);
    characterListView->addEventListener([=](Ref*, ScrollView::EventType eventType) {
      
        if( eventType == ScrollView::EventType::AUTOSCROLL_ENDED ) {
            // CCLOG("AUTOSCROLL_ENDED");
            // characterListView->setBounceEnabled(true);
        }
    });
    
    addContentChild(characterListView);
    
    auto characters = CHARACTER_MANAGER->getCharacters();
    
    for( int i = 0; i < characters.size() ; ++i ) {
        auto cell = CharacterCell::create(characters[i]);
        cell->setAnchorPoint(ANCHOR_M);
        characterListView->pushBackCustomItem(cell);
        
        cell->setOnSelectListener([=](CharacterCell *cell) {
            CHARACTER_MANAGER->setSelected(cell->getData().charId);
            
            auto items = characterListView->getItems();
            
            for( auto item : items ) {
                dynamic_cast<CharacterCell*>(item)->unselect();
            }
            
            cell->select();
        });
        
        cell->setOnViewAdsListener([=](CharacterCell *cell) {
            CHARACTER_MANAGER->increaseViewAdsCount(cell->getData().charId);
            cell->updateUnlockAmount();
            
            CHARACTER_MANAGER->checkUnlock([=](CharacterDataList unlockCharacters) {
                
                // TODO: 캐릭터 획득 팝업
                
                cell->unlock();
            });
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
 * 등장 연출
 */
void ShopPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(0/* ACTION DURATION */, onFinished);
}

/**
 * 등장 연출 완료
 */
void ShopPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
}
