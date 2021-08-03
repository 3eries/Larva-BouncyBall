//
//  FirebaseManager.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "FirebaseManager.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

static FirebaseManager *instance = nullptr;
FirebaseManager* FirebaseManager::getInstance() {
    
    if( !instance ) {
        instance = new FirebaseManager();
    }
    
    return instance;
}

void FirebaseManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

FirebaseManager::FirebaseManager() {
}

FirebaseManager::~FirebaseManager() {
}

NS_SB_END;
