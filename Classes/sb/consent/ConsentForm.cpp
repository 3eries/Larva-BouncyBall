//
//  ConsentForm.cpp
//
//  Created by seongmin hwang on 2018. 9. 26..
//

#include "ConsentForm.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

ConsentForm* ConsentForm::create() {
    
    auto form = new ConsentForm();
    
    if( form && form->init() ) {
        form->autorelease();
        return form;
    }
    
    CC_SAFE_DELETE(form);
    return nullptr;
}

ConsentForm::ConsentForm() {
    
}

ConsentForm::~ConsentForm() {
    
}

bool ConsentForm::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    return true;
}

NS_SB_END;
