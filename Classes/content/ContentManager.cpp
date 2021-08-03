//
//  ContentManager.cpp
//
//  Created by seongmin hwang on 07/02/2019.
//

#include "ContentManager.hpp"

#include "Define.h"

USING_NS_CC;
USING_NS_SB;
using namespace std;

static ContentManager *instance = nullptr;
ContentManager* ContentManager::getInstance() {
    
    if( !instance ) {
        instance = new ContentManager();
    }
    
    return instance;
}

void ContentManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

ContentManager::ContentManager() :
db(nullptr) {
    
}

ContentManager::~ContentManager() {
    
}

void ContentManager::init() {
    
    ContentResourceHelper::getInstance();
    
    db = Database::getInstance();
    db->init();
}

Database* ContentManager::getDatabase() {
    return instance->db;
}
