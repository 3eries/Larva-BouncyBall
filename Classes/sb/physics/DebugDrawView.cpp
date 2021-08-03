//
//  DebugDrawView.cpp
//
//  Created by seongmin hwang on 16/11/2018.
//

#include "DebugDrawView.hpp"

#include "SBPhysics.hpp"

USING_NS_CC;
using namespace std;

DebugDrawView* DebugDrawView::create(b2World *world) {
    
    auto view = new DebugDrawView();
    
    if( view && view->init(world) ) {
        view->autorelease();
        return view;
    }
    
    delete view;
    return nullptr;
}

DebugDrawView::DebugDrawView() :
debugDrawFlag(true),
world(nullptr),
debugDraw(nullptr) {
    
}

DebugDrawView::~DebugDrawView() {
 
    CC_SAFE_DELETE(debugDraw);
}

bool DebugDrawView::init(b2World *world) {
    
    if( !Node::init() ) {
        return false;
    }
    
    this->world = world;
    
    debugDraw = new GLESDebugDraw(PTM_RATIO);
    world->SetDebugDraw(debugDraw);
    
    return true;
}

void DebugDrawView::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    
    Node::draw(renderer, transform, flags);
    
    _customCmd.init(_globalZOrder, transform, flags);
    _customCmd.func = CC_CALLBACK_0(DebugDrawView::onDraw, this, transform, flags);
    renderer->addCommand(&_customCmd);
}

void DebugDrawView::onDraw(const Mat4 &transform, uint32_t flags) {
    
    auto director = Director::getInstance();
    CCASSERT(director != nullptr, "Director is null when setting matrix stack");
    
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
    world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
