//
//  ContentResourceHelper.cpp
//
//  Created by seongmin hwang on 07/02/2019.
//

#include "ContentResourceHelper.hpp"

#include "Define.h"
#include "data/Database.hpp"
#include "User.hpp"

USING_NS_CC;
using namespace std;

static ContentResourceHelper *instance = nullptr;
ContentResourceHelper* ContentResourceHelper::getInstance() {
    
    if( !instance ) {
        instance = new ContentResourceHelper();
    }
    
    return instance;
}

void ContentResourceHelper::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

ContentResourceHelper::ContentResourceHelper() {
}

ContentResourceHelper::~ContentResourceHelper() {
}

/**
 * 컨텐츠 리소스 preload
 */
void ContentResourceHelper::preload() {
}
