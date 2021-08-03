//
//  TestHelper.cpp
//
//  Created by seongmin hwang on 2018. 6. 1..
//

#include "TestHelper.hpp"

#include "GameConfiguration.hpp"

USING_NS_CC;
using namespace std;

static TestHelper *instance = nullptr;
TestHelper* TestHelper::getInstance() {
    
    if( !instance ) {
        instance = new TestHelper();
    }
    
    return instance;
}

void TestHelper::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

TestHelper::TestHelper() :
cheatMode(false) {
}

TestHelper::~TestHelper() {
    
}
