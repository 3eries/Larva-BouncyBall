//
//  BannerView.hpp
//
//  Created by seongmin hwang on 27/03/2019.
//

#ifndef BannerView_hpp
#define BannerView_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

/** @class BannerView
 * @brief 배너 광고 Wrapper 클래스입니다
 */
class BannerView : public cocos2d::Node {
public:
    CREATE_FUNC(BannerView);
    ~BannerView();
    
private:
    BannerView();
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void cleanup() override;
    
public:
    void setVisible(bool isVisible) override;
    void updateNativeUI();
    
private:
    SBButton *moreGames;
};

#endif /* BannerView_hpp */
