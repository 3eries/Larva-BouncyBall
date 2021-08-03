//
//  SBNodeUtils.cpp
//
//  Created by hwangseongmin on 2016. 11. 29..
//
//

#include "SBNodeUtils.hpp"

#include "../base/SBMacros.h"
#include "../base/SBTypes.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

Node* SBNodeUtils::createZeroSizeNode(Node *n) {
    
    n->setAnchorPoint(Vec2::ZERO);
    n->setPosition(Vec2::ZERO);
    n->setContentSize(Size::ZERO);
    
    return n;
}

Node* SBNodeUtils::createWinSizeNode(Node *n) {
    
    n->setAnchorPoint(Vec2::ZERO);
    n->setPosition(Vec2::ZERO);
    n->setContentSize(SB_WIN_SIZE);
    
    return n;
}

Widget* SBNodeUtils::createTouchNode() {
    
    auto n = Widget::create();
    n->setAnchorPoint(Vec2::ZERO);
    n->setPosition(Vec2::ZERO);
    n->setContentSize(SB_WIN_SIZE);
    n->setTouchEnabled(true);
    
    return n;
}

Widget* SBNodeUtils::createTouchNode(Color4B color) {
    
    auto n = createTouchNode();
    n->addChild(LayerColor::create(color));
    
    return n;
}

Node* SBNodeUtils::createSwallowMultiTouchNode() {
    
    auto n = SBNodeUtils::createZeroSizeNode();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [=](Touch *touch, Event *unusedEvent) -> bool {
        return touch->getID() > 0;
    };
    n->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, n);
    
    return n;
}

Node* SBNodeUtils::createBackgroundNode(Node *parent, Color4B color) {
    
    auto n = LayerColor::create(color);
    n->setIgnoreAnchorPointForPosition(false);
    n->setAnchorPoint(Vec2::ZERO);
    n->setPosition(Vec2::ZERO);
    n->setContentSize(parent->getContentSize());
    
    return n;
}

void SBNodeUtils::scale(Node *target, const Size &targetSize) {
    
    target->setScaleX(targetSize.width / target->getContentSize().width);
    target->setScaleY(targetSize.height / target->getContentSize().height);
}

void SBNodeUtils::alignCenterHorizontal(const Size &size, vector<Node*> nodes) {
    
    float min = INT_MAX;
    float max = INT_MIN;
    
    for( auto n : nodes ) {
        auto box = getBoundingBoxInWorld(n);
        
        if( box.getMinX() < min )   min = box.getMinX();
        if( box.getMaxX() > max )   max = box.getMaxX();
    }
    
    float w = max - min;
    float x = (size.width*0.5f) - (w*0.5f);
    float diff = x - min;
    
    for( auto n : nodes ) {
        n->setPositionX(n->getPositionX() + diff);
    }
}

void SBNodeUtils::alignCenterHorizontal(vector<Node*> nodes) {
    
    alignCenterHorizontal(SB_WIN_SIZE, nodes);
}

void SBNodeUtils::recursiveResume(Node *n) {
    
    n->resume();
    
    auto children = n->getChildren();
    if( children.size() == 0 ) {
        return;
    }
    
    for( auto child : children ) {
        recursiveResume(child);
    }
}

void SBNodeUtils::recursivePause(Node *n) {
    
    n->pause();
    
    auto children = n->getChildren();
    if( children.size() == 0 ) {
        return;
    }
    
    for( auto child : children ) {
        recursivePause(child);
    }
}

void SBNodeUtils::recursiveResumeSchedulerAndActions(Node *n) {
    
    n->getScheduler()->resumeTarget(n);
    n->getActionManager()->resumeTarget(n);
    
    auto children = n->getChildren();
    if( children.size() == 0 ) {
        return;
    }
    
    for( auto child : children ) {
        recursiveResumeSchedulerAndActions(child);
    }
}

void SBNodeUtils::recursivePauseSchedulerAndActions(Node *n) {
    
    n->getScheduler()->pauseTarget(n);
    n->getActionManager()->pauseTarget(n);
    
    auto children = n->getChildren();
    if( children.size() == 0 ) {
        return;
    }
    
    for( auto child : children ) {
        recursivePauseSchedulerAndActions(child);
    }
}

void SBNodeUtils::recursiveCascadeOpacityEnabled(Node *n, bool enabled) {
    
    n->setCascadeOpacityEnabled(enabled);
    
    auto children = n->getChildren();
    if( children.size() == 0 ) {
        return;
    }
    
    for( auto child : children ) {
        recursiveCascadeOpacityEnabled(child, enabled);
    }
}

void SBNodeUtils::recursiveCascadeColorEnabled(Node *n, bool enabled) {
    
    n->setCascadeColorEnabled(enabled);
    
    auto children = n->getChildren();
    if( children.size() == 0 ) {
        return;
    }
    
    for( auto child : children ) {
        recursiveCascadeColorEnabled(child, enabled);
    }
}

/**
 *  parent의 visible 여부를 반환한다.
 */
bool SBNodeUtils::hasVisibleParents(Node *n) {
    
    for( auto c = n; c != nullptr; c = c->getParent() ) {
        if( !c->isVisible() ) {
            return false;
        }
    }
    
    return true;
}

/**
 * 노드의 화면상에 절대 좌표를 반환합니다
 */
Rect SBNodeUtils::getBoundingBoxInWorld(Node *n, float scaleX, float scaleY) {
    
    /*
     if( !n->isRunning() || !n->getParent() ) {
     Point p = n->getPosition();
     Size s = n->getContentSize();
     s.width *= scaleX;
     s.height *= scaleY;
     
     return Rect(p.x - n->getAnchorPoint().x * s.width,
     p.y - n->getAnchorPoint().y * s.height,
     s.width,
     s.height);
     
     } else {
     Point p = n->getParent()->convertToWorldSpace(n->getPosition());
     Size s = n->getContentSize();
     s.width *= scaleX;
     s.height *= scaleY;
     
     return Rect(p.x - n->getAnchorPoint().x * s.width,
     p.y - n->getAnchorPoint().y * s.height,
     s.width,
     s.height);
     }
     */
    
    Point pos = n->getPosition();
    Size size = n->getContentSize();
    
    if( n->isRunning() && n->getParent() ) {
        pos = n->getParent()->convertToWorldSpace(pos);
    }
    
    pos -= n->getAnchorPointInPoints(); // calculate left bottom
    pos += Point(size*0.5f);            // calculate middle
    
    // apply scale to size
    size.width *= scaleX;
    size.height *= scaleY;
    
    // calculate new left bottom
    pos -= Point(size*0.5f);
    
    return Rect(pos.x, pos.y, size.width, size.height);
}

Rect SBNodeUtils::getBoundingBoxInWorld(Node *n, bool scale) {

    if( scale ) {
        return getBoundingBoxInWorld(n, n->getScaleX(), n->getScaleY());
    } else {
        return getBoundingBoxInWorld(n, 1, 1);
    }
}

/**
 * 노드의 자식 좌표, 크기를 계산하여 노드의 크기를 반환한다.
 */
Size SBNodeUtils::getChildrenBoundingSize(Node *n) {
    
    Vec2 min(INT_MAX, INT_MAX);
    Vec2 max(INT_MIN, INT_MIN);
    
    auto children = n->getChildren();
    
    for( auto child : children ) {
        Rect r = getBoundingBoxInWorld(child);
        
        min.x = MIN(r.getMinX(), min.x);
        min.y = MIN(r.getMinY(), min.y);
        max.x = MAX(r.getMaxX(), max.x);
        max.y = MAX(r.getMaxY(), max.y);
    }
    
    return Size(max - min);
}

/**
 *  @return 노드 영역 내에 좌표 포함 여부
 */
bool SBNodeUtils::isPointInside(Node *n, Point point,
                                 bool isApplyAffineTransform) {
    
    if( isApplyAffineTransform ) {
        return containsPoint(getPolygon(n), point);
        
    } else {
        return getBoundingBoxInWorld(n).containsPoint(point);
    }
}

/**
 *  @return 노드 영역 내에 좌표 포함 여부
 */
bool SBNodeUtils::isTouchInside(Node *n, Touch *touch, bool isApplyAffineTransform) {
    
    return isPointInside(n, touch->getLocation(), isApplyAffineTransform);
}

void SBNodeUtils::setButtonImage(Button *btn,
                                 const string &normalImage,
                                 const string &selectedImage,
                                 const string &disableImage) {
    
    if( btn ) {
        btn->loadTextures(normalImage, selectedImage, disableImage);
    }
}

Animation* SBNodeUtils::createAnimation(vector<string> animFiles,
                                        float delayPerUnit) {
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(delayPerUnit);
    
    for( int i = 0; i < animFiles.size(); ++i ) {
        anim->addSpriteFrameWithFile(animFiles[i]);
    }
    
    return anim;
}

/**
 *  다각형에 좌표 포함 여부
 */
bool SBNodeUtils::containsPoint(vector<Point> polygon, Point p)  {
    
    int j = (int)polygon.size()-1;
    bool oddNodes = false;
    
    for( int i = 0; i < polygon.size(); i++ ) {
        
        if( (polygon[i].y < p.y && polygon[j].y >= p.y) ||
           (polygon[j].y < p.y && polygon[i].y >= p.y) ) {
            
            if( polygon[i].x + (p.y - polygon[i].y) /
               (polygon[j].y - polygon[i].y) *
               (polygon[j].x - polygon[i].x) < p.x ) {
                
                oddNodes=!oddNodes;
            }
        }
        
        j = i;
    }
    
    return oddNodes;
}

bool SBNodeUtils::containsPoint(Polygon polygon, Rect rect) {
    
    Polygon polygon2;
    polygon2.push_back(Point(rect.getMinX(), rect.getMinY()));
    polygon2.push_back(Point(rect.getMaxX(), rect.getMinY()));
    polygon2.push_back(Point(rect.getMaxX(), rect.getMaxY()));
    polygon2.push_back(Point(rect.getMinX(), rect.getMaxY()));
    
    return containsPoint(polygon, polygon2);
}

bool SBNodeUtils::containsPoint(Polygon p1, Polygon p2) {
    
    for( int i = 0; i < p2.size(); i++ ) {
        if( containsPoint(p1, p2[i]) ) {
            return true;
        }
    }
    
    return false;
}

SBNodeUtils::Polygon SBNodeUtils::getPolygon(Node *n) {
    
    Size size = n->getContentSize();
//        const Mat4 &m = n->getNodeToParentTransform();
    auto transform = n->getNodeToParentAffineTransform();
    
    Point pos[] = {
        Point(0,0),
        Point(size.width,0),
        Point(size.width,size.height),
        Point(0,size.height),
    };
    
    // 반시계 방향으로 초기화
    Polygon polygon;
    
    for( int i = 0; i < sizeof(pos) / sizeof(Point); ++i ) {
        polygon.push_back(pos[i]);
    }
    
    // 트랜스폼 값 적용하여 좌표 재설정
    for( int i = 0; i < polygon.size(); ++i ) {
        Point p = PointApplyAffineTransform(polygon[i], transform);
        
        // 절대좌표로 컨버팅
        polygon[i] = n->getParent()->convertToWorldSpace(p);
    }
    
    return polygon;
}
