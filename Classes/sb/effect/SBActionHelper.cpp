//
//  SBActionHelper.cpp
//
//  Created by seongmin hwang on 2018. 7. 16..
//

#include "SBActionHelper.hpp"

USING_NS_CC;

void SBActionHelper::runMoveAction(Node *n, Vec2 from, Vec2 to, float duration,
                                   bool isClear) {
    
    if( isClear ) {
        n->stopAllActions();
    }
    
    n->setPosition(from);
    n->runAction(MoveTo::create(duration, to));
}
