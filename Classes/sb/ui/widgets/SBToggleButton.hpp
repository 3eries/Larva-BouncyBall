//
//  SBToggleButton.hpp
//
//  Created by seongmin hwang on 2018. 3. 2..
//

#ifndef SBToggleButton_hpp
#define SBToggleButton_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../../base/SBMacros.h"

class SBToggleButton : public cocos2d::ui::Button {
public:
    typedef std::function<bool(bool)> OnSelectedListener;
    
public:
    static SBToggleButton* create(const std::string &unselectedFile,
                                  const std::string &selectedFile,
                                  const std::string &disabledFile = "");
    virtual ~SBToggleButton();
    
protected:
    SBToggleButton();
    
    virtual bool init(const std::string &unselectedFile,
                      const std::string &selectedFile,
                      const std::string &disabledFile);
    
    virtual void releaseUpEvent() override;
    
public:
    void setSelected(bool isSelected);
    
protected:
    std::string unselectedFile;
    std::string selectedFile;
    std::string disabledFile;
    
    SB_SYNTHESIZE_READONLY_BOOL(selected, Selected);
    CC_SYNTHESIZE(OnSelectedListener, onSelectedListener, OnSelectedListener);
};

#endif /* SBToggleButton_hpp */
