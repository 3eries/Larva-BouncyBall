//
//  CharacterListener.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/17.
//

#ifndef CharacterListener_hpp
#define CharacterListener_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "data/model/CharacterData.h"

class CharacterListener : public cocos2d::Ref {
public:
    static CharacterListener* create(cocos2d::Ref *target);
    CharacterListener(cocos2d::Ref *target);
    ~CharacterListener();
    
    OnCharacterListener         onCharacterSelected;       // 캐릭터 선택됨
    OnCharacterListListener     onCharacterUnlocked;       // 캐릭터 잠금 해제
    OnCharacterListListener     onCharacterRestored;       // 캐릭터 IAP 복원
private:
    CC_SYNTHESIZE(cocos2d::Ref*, target, Target);
};

#endif /* CharacterListener_hpp */
