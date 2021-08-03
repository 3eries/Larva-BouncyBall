//
//  DebugDrawView.hpp
//
//  Created by seongmin hwang on 16/11/2018.
//

#ifndef DebugDrawView_hpp
#define DebugDrawView_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "GLES-Render.h"

class DebugDrawView : public cocos2d::Node {
public:
    static DebugDrawView* create(b2World *world);
    ~DebugDrawView();
    
private:
    DebugDrawView();
    
    bool init(b2World *world);
    
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    
private:
    bool debugDrawFlag;
    
    b2World *world;
    CC_SYNTHESIZE_READONLY(GLESDebugDraw*, debugDraw, DebugDraw);
    cocos2d::CustomCommand _customCmd;
};

#endif /* DebugDrawView_hpp */
