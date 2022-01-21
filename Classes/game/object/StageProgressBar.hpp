//
//  StageProgressBar.hpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#ifndef StageProgressBar_hpp
#define StageProgressBar_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "../GameManager.hpp"
#include "../../content/data/model/StageData.h"

class StageProgressBar : public cocos2d::Node {
public:
    CREATE_FUNC(StageProgressBar);
    ~StageProgressBar();
    
private:
    StageProgressBar();
    
    bool init() override;
    
    void initUI();
    void initGameListener();
    
public:
    void setStar(int star);
    
private:
    std::vector<cocos2d::Sprite*> stars;
};

#endif /* StageProgressBar_hpp */
