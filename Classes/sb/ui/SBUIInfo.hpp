//
//  SBUIInfo.hpp
//  Snow Bros-mobile
//
//  Created by seongmin hwang on 2018. 3. 7..
//

#ifndef SBUIInfo_hpp
#define SBUIInfo_hpp

#include <stdio.h>

#include "cocos2d.h"

struct CC_DLL SBUIInfo {
    static const int DEFAULT_ZORDER = 0;
    
    explicit SBUIInfo();
    explicit SBUIInfo(int _tag, int _zOrder,
                      cocos2d::Vec2 _anchorPoint, cocos2d::Vec2 _pos,
                      const std::string &_file);
    explicit SBUIInfo(int _tag, int _zOrder);
    explicit SBUIInfo(int _tag, const std::string &_file);
    explicit SBUIInfo(int _tag, const cocos2d::Vec2 _anchorPoint, cocos2d::Vec2 _pos,
                      const std::string &_file, bool _visible);
    explicit SBUIInfo(int _tag, const cocos2d::Vec2 _anchorPoint, cocos2d::Vec2 _pos,
                      const std::string &_file);
    explicit SBUIInfo(const cocos2d::Vec2 _anchorPoint, cocos2d::Vec2 _pos, const std::string &_file);
    explicit SBUIInfo(const std::string &_file);
    
    int tag;
    int zOrder;
    cocos2d::Vec2 anchorPoint;
    cocos2d::Vec2 pos;
    std::string file;
    bool visible;
    
    void apply(cocos2d::Node *target);
};

#endif /* SBUIInfo_hpp */
