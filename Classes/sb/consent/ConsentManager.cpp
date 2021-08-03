//
//  ConsentManager.cpp
//
//  Created by seongmin hwang on 2018. 9. 27..
//

#include "ConsentManager.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

static const char* USER_DEFAULT_KEY_PRIVACY_POLICY = "SUPERBOMB_USER_DEFAULT_KEY_PRIVACY_POLICY";

static ConsentManager *instance = nullptr;
ConsentManager* ConsentManager::getInstance() {
    
    if( !instance ) {
        instance = new ConsentManager();
    }
    
    return instance;
}

void ConsentManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

ConsentManager::ConsentManager() {
    
}

ConsentManager::~ConsentManager() {
    
}

void ConsentManager::setPrivacyPolicyChecked(bool isChecked) {
    
    UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_KEY_PRIVACY_POLICY, isChecked);
    UserDefault::getInstance()->flush();
}

bool ConsentManager::isPrivacyPolicyChecked() {
    
    return UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_KEY_PRIVACY_POLICY, false);
}

NS_SB_END;
