//
//  SBNodeUtils.hpp
//
//  Created by hwangseongmin on 2016. 11. 29..
//
//

#ifndef SBNodeUtils_hpp
#define SBNodeUtils_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SBNodeUtils {
public:
    template <typename T>
    static T findRunningScene() {
        auto cast = [](cocos2d::Scene *scene) {
            return dynamic_cast<T>(scene);
        };
        
        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        
        if( cast(runningScene) ) {
            return (T)runningScene;
        }
        
        auto trans = dynamic_cast<cocos2d::TransitionScene*>(runningScene);
        
        if( trans && cast(trans->getInScene()) ) {
            return (T)trans->getInScene();
        }
        
        return nullptr;
    }
    
public:
    static cocos2d::Node* createZeroSizeNode(cocos2d::Node *n = cocos2d::Node::create());
    static cocos2d::Node* createWinSizeNode(cocos2d::Node *n = cocos2d::Node::create());
    static cocos2d::ui::Widget* createTouchNode();
    static cocos2d::ui::Widget* createTouchNode(cocos2d::Color4B color);
    static cocos2d::Node* createSwallowMultiTouchNode();
    static cocos2d::Node* createBackgroundNode(cocos2d::Node *parent, cocos2d::Color4B color);
 
    static void scale(cocos2d::Node *target, const cocos2d::Size &targetSize);
    
    static void alignCenterHorizontal(const cocos2d::Size &size,
                                      std::vector<cocos2d::Node*> nodes);
    static void alignCenterHorizontal(std::vector<cocos2d::Node*> nodes);
    
    static void recursiveResume(cocos2d::Node *n);
    static void recursivePause(cocos2d::Node *n);
    
    static void recursiveResumeSchedulerAndActions(cocos2d::Node *n);
    static void recursivePauseSchedulerAndActions(cocos2d::Node *n);

    static void recursiveCascadeOpacityEnabled(cocos2d::Node *n, bool enabled);
    static void recursiveCascadeColorEnabled(cocos2d::Node *n, bool enabled);
    
    static bool hasVisibleParents(cocos2d::Node *n);
    
    static cocos2d::Rect getBoundingBoxInWorld(cocos2d::Node *n,
                                               float scaleX = 1, float scaleY = 1);
    static cocos2d::Rect getBoundingBoxInWorld(cocos2d::Node *n, bool scale);
    static cocos2d::Size getChildrenBoundingSize(cocos2d::Node *n);
    
    static bool isPointInside(cocos2d::Node *n, cocos2d::Point point,
                              bool isApplyAffineTransform = false);
    static bool isTouchInside(cocos2d::Node *n, cocos2d::Touch *touch,
                              bool isApplyAffineTransform = false);
    
    static void setButtonImage(cocos2d::ui::Button *btn,
                               const std::string &normalImage,
                               const std::string &selectedImage = "",
                               const std::string &disableImage = "");
    static cocos2d::Animation* createAnimation(std::vector<std::string> animFiles,
                                               float delayPerUnit);
    
public:
    typedef std::vector<cocos2d::Point> Polygon;
    
    static bool containsPoint(std::vector<cocos2d::Point> polygon, cocos2d::Point p);
    static bool containsPoint(Polygon polygon, cocos2d::Rect rect);
    static bool containsPoint(Polygon p1, Polygon p2);
    static SBNodeUtils::Polygon getPolygon(cocos2d::Node *n);
};

#endif /* SBNodeUtils_hpp */
