//
//  CharacterListener.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/17.
//

#include "CharacterListener.hpp"

USING_NS_CC;

CharacterListener* CharacterListener::create(Ref *target) {
    
    auto listener = new CharacterListener(target);
    
    if( listener ) {
        listener->autorelease();
        return listener;
    }
    
    delete listener;
    return nullptr;
}

CharacterListener::CharacterListener(Ref *target) :
target(target),
onCharacterSelected(nullptr),
onCharacterUnlocked(nullptr),
onCharacterRestored(nullptr) {
}

CharacterListener::~CharacterListener() {
}
