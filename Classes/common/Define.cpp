//
//  Define.cpp
//
//  Created by seongmin hwang
//

#include "Define.h"

#include "SceneManager.h"
#include "PopupManager.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

void removeListeners(Ref *target) {
    
    auto node = dynamic_cast<Node*>(target);
    if( node ) {
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node);
    }
    
    iap::IAPHelper::getInstance()->removeListener(target);
    AdsHelper::getInstance()->getEventDispatcher()->removeListener(target);
    PopupManager::getInstance()->removeListener(target);
}
