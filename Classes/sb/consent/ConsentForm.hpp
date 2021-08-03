//
//  ConsentForm.hpp
//
//  Created by seongmin hwang on 2018. 9. 26..
//

#ifndef ConsentForm_hpp
#define ConsentForm_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../base/SBMacros.h"

NS_SB_BEGIN;

class ConsentForm : public cocos2d::Node {
public:
    static ConsentForm* create();
    virtual ~ConsentForm();
    
private:
    ConsentForm();
    virtual bool init() override;
    
public:
    static void show();
};

NS_SB_END;

#endif /* ConsentForm_hpp */
