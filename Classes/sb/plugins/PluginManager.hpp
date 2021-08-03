//
//  PluginManager.hpp
//
//  Created by seongmin hwang on 2018. 7. 20..
//

#ifndef PluginManager_hpp
#define PluginManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../base/SBMacros.h"
#include "../base/SBTypes.hpp"

#include "json/document-wrapper.h"

NS_SB_BEGIN;

class PluginManager {
    public:
    static PluginManager* getInstance();
    static void destroyInstance();
    ~PluginManager();
    
private:
    PluginManager();
    
    void initAds(const rapidjson::Value &obj);
    void initIAP(const rapidjson::Value &obj);
    
public:
    void init(const std::string &json);
};

NS_SB_END;

#endif /* PluginManager_hpp */
