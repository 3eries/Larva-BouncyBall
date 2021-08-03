//
//  SBToggleButton.cpp
//
//  Created by seongmin hwang on 2018. 3. 2..
//

#include "SBToggleButton.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

SBToggleButton* SBToggleButton::create(const string &unselectedFile,
                                       const string &selectedFile,
                                       const string &disabledFile) {
    
    auto btn = new SBToggleButton();
    
    if( btn && btn->init(unselectedFile, selectedFile, disabledFile) ) {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return nullptr;
}

SBToggleButton::SBToggleButton() :
selected(false),
onSelectedListener(nullptr) {
    
}

SBToggleButton::~SBToggleButton() {
    
}

bool SBToggleButton::init(const string &unselectedFile,
                          const string &selectedFile,
                          const string &disabledFile) {

    if( !Button::init(unselectedFile, "", disabledFile, TextureResType::LOCAL) ) {
        return false;
    }
    
    this->unselectedFile = unselectedFile;
    this->selectedFile = selectedFile;
    this->disabledFile = disabledFile;
    
    return true;
}

void SBToggleButton::releaseUpEvent() {
    
    Button::releaseUpEvent();
    
    retain();
    
    bool isSelected = !this->selected;
    
    if( onSelectedListener ) {
        if( !onSelectedListener(isSelected) ) {
            setSelected(isSelected);
        }
    } else {
        setSelected(isSelected);
    }
    
    release();
}

void SBToggleButton::setSelected(bool isSelected) {
    
    this->selected = isSelected;
    
    Button::loadTextureNormal(isSelected ? selectedFile : unselectedFile);
}
