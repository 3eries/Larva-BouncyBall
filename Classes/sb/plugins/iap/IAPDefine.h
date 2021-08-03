//
//  IAPDefine.h
//
//  Created by seongmin hwang on 2018. 7. 12..
//

#ifndef IAPDefine_h
#define IAPDefine_h

#include "../../base/SBMacros.h"

NS_SB_BEGIN;

#ifdef __cplusplus
#define NS_IAP_BEGIN                     namespace iap {
#define NS_IAP_END                       }
#else
#define NS_SB_BEGIN
#define NS_SB_END
#endif

NS_IAP_BEGIN;

enum class ItemType {
    CONSUMABLE,
    NON_CONSUMABLE,
    REMOVE_ADS,
};

struct Item {
    std::string name; // json key
    ItemType type;
    std::string itemId;
    int amount;
    std::string price;
    
    Item() : name(""), type(ItemType::CONSUMABLE), itemId(""), amount(0), price("") {
    }
};

typedef std::vector<Item> ItemList;

struct Config {
    std::vector<Item> items;
};

NS_IAP_END;
NS_SB_END;

#endif /* IAPDefine_h */
