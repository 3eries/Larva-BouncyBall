//
//  PluginManager.cpp
//
//  Created by seongmin hwang on 2018. 7. 20..
//

#include "PluginManager.hpp"

#include "json/stringbuffer.h"
#include "json/prettywriter.h"

#include "../utils/SBStringUtils.h"

#include "firebase/FirebaseManager.hpp"
#include "analytics/SBAnalytics.hpp"
#include "ads/AdsHelper.hpp"
#include "iap/IAPHelper.hpp"
#include "play/PluginPlay.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

static PluginManager *instance = nullptr;
PluginManager* PluginManager::getInstance() {
    
    if( !instance ) {
        instance = new PluginManager();
    }
    
    return instance;
}

void PluginManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

PluginManager::PluginManager() {
}

PluginManager::~PluginManager() {
}

void PluginManager::init(const string &json) {
 
    CCLOG("PluginManager::init:\n%s", json.c_str());
    
    // parse json
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto obj = doc["ios"].GetObject();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto obj = doc["android"].GetObject();
#endif
    
#if SB_PLUGIN_USE_PLAY
    PluginPlay::getInstance()->init();
#endif
    
#if SB_PLUGIN_USE_FIREBASE
    FirebaseManager::getInstance()->init();
#endif
    
    SBAnalytics::init();
    
#if SB_PLUGIN_USE_ADS
    if( obj.HasMember("ads") )       initAds(obj["ads"]);
#endif
    
#if SB_PLUGIN_USE_IAP
    if( obj.HasMember("iap") )       initIAP(obj["iap"]);
#endif
}

/**
 * 광고 모듈 초기화
 */
void PluginManager::initAds(const rapidjson::Value &obj) {
    
    AdsConfig adsConfig;
    
    // test value
    {
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
        adsConfig.testMode = (obj["test"].GetInt() != 0);
#elif COCOS2D_DEBUG == 1
        adsConfig.testMode = true;
#endif
    }
    
    // auto load interval
    if( obj.HasMember("auto_load_interval") ) {
        adsConfig.autoLoadInterval = obj["auto_load_interval"].GetFloat();
    }
    
    // app id
    adsConfig.appId = obj["app_id"].GetString();
    
    // ad unit id
    if( adsConfig.testMode ) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // Banner                        ca-app-pub-3940256099942544/2934735716
        // Interstitial                  ca-app-pub-3940256099942544/4411468910
        // Interstitial Video            ca-app-pub-3940256099942544/5135589807
        // Rewarded Video                ca-app-pub-3940256099942544/1712485313
        // Native Advanced               ca-app-pub-3940256099942544/3986624511
        // Native Advanced Video         ca-app-pub-3940256099942544/2521693316
        adsConfig.bannerUnitId        = "ca-app-pub-3940256099942544/2934735716";
        adsConfig.interstitialUnitId  = "ca-app-pub-3940256099942544/4411468910";
        adsConfig.rewardedVideoUnitId = "ca-app-pub-3940256099942544/1712485313";
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        // Banner                        ca-app-pub-3940256099942544/6300978111
        // Interstitial                  ca-app-pub-3940256099942544/1033173712
        // Interstitial Video            ca-app-pub-3940256099942544/8691691433
        // Rewarded Video                ca-app-pub-3940256099942544/5224354917
        // Native Advanced               ca-app-pub-3940256099942544/2247696110
        // Native Advanced Video         ca-app-pub-3940256099942544/1044960115
        adsConfig.bannerUnitId        = "ca-app-pub-3940256099942544/6300978111";
        adsConfig.interstitialUnitId  = "ca-app-pub-3940256099942544/1033173712";
        adsConfig.rewardedVideoUnitId = "ca-app-pub-3940256099942544/5224354917";
#endif
        
    } else {
        adsConfig.bannerUnitId        = obj.HasMember("banner_unit_id") ? obj["banner_unit_id"].GetString() : "";
        adsConfig.interstitialUnitId  = obj.HasMember("interstitial_unit_id") ? obj["interstitial_unit_id"].GetString() : "";
        adsConfig.rewardedVideoUnitId = obj.HasMember("rewarded_video_unit_id") ? obj["rewarded_video_unit_id"].GetString() : "";
    }
    
    AdsHelper::getInstance()->init(adsConfig);
}

/**
 * IAP 모듈 초기화
 */
void PluginManager::initIAP(const rapidjson::Value &obj) {
    
    iap::Config config;
    
    // items
    auto items = obj["items"].GetObject();
    
    for( auto it = items.MemberBegin(); it != items.MemberEnd(); ++it ) {
        auto itemObj = it->value.GetObject();
        
        iap::Item item;
        item.name = it->name.GetString();
        item.itemId = itemObj["id"].GetString();
        item.amount = itemObj["amount"].GetInt();
        item.price  = itemObj["price"].GetString();
        
        if( itemObj.HasMember("type") ) {
            string type = itemObj["type"].GetString();
            
            if( type == "consumable" )          item.type = iap::ItemType::CONSUMABLE;
            else if( type == "non_consumable" ) item.type = iap::ItemType::NON_CONSUMABLE;
            else if( type == "remove_ads" )     item.type = iap::ItemType::REMOVE_ADS;
        }
        
        config.items.push_back(item);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    rapidjson::StringBuffer strbuf;
    strbuf.Clear();
    
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    obj.Accept(writer);
    
    string json = strbuf.GetString();
    iap::IAPHelper::getInstance()->init(config, json);
    
#else
    iap::IAPHelper::getInstance()->init(config);
#endif
}

NS_SB_END;
