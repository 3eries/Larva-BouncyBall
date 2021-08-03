//
//  PluginPlay.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "PluginPlay.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

static PluginPlay *instance = nullptr;
PluginPlay* PluginPlay::getInstance() {
    
    if( !instance ) {
        instance = new PluginPlay();
    }
    
    return instance;
}

void PluginPlay::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

PluginPlay::PluginPlay() {
}

PluginPlay::~PluginPlay() {
}

NS_SB_END;
