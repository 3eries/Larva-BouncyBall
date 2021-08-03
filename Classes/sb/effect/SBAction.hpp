//
//  SBAction.hpp
//
//  Created by seongmin hwang on 2018. 7. 4..
//

#ifndef SBAction_hpp
#define SBAction_hpp

#include <stdio.h>

#include "cocos2d.h"

/**
 * @class SBRollingNumber
 */
class SBRollingNumber : public cocos2d::ActionInterval {
public:
    typedef std::function<void(std::string)> OnRollingListener;
    
    static const std::string SCHEDULER_ROLLING;
    static const std::string SCHEDULER_ROLLING_STOP;
    
public:
    static SBRollingNumber* create(OnRollingListener onRollingListener, int number,
                                   float timePerDigit, float rollSpeed = 0.06f);
    virtual ~SBRollingNumber();
    
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void stop() override;
    
    virtual void update(float t) override;
    
protected:
    SBRollingNumber();
    
    virtual bool init(OnRollingListener onRollingListener, int number,
                      float timePerDigit, float rollSpeed);
    
    virtual void onRolling();
    virtual void onRollingFinished();
    
    void performListener();
    
    bool isRollingFinished();
    
protected:
    OnRollingListener onRollingListener;
    int number;
    float timePerDigit;
    float rollSpeed;
    
    std::vector<std::string> numberList;
    std::vector<std::string> rollNumberList;
    int rollIndex;
};

/**
 * @class SBIncreaseNumber
 */
class SBIncreaseNumber : public cocos2d::ActionInterval {
public:
    typedef std::function<void(std::string)> OnIncreaseListener;
    
    static const std::string SCHEDULER_INCREASE;
    
public:
    static SBIncreaseNumber* create(OnIncreaseListener onIncreaseListener, float duration,
                                    int startNumber, int targetNumber);
    virtual ~SBIncreaseNumber();
    
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void stop() override;
    
    virtual void update(float t) override;
    
protected:
    SBIncreaseNumber();
    
    virtual bool init(OnIncreaseListener onIncreaseListener, float duration,
                      int startNumber, int targetNumber);
    
    virtual void onIncrease();
    virtual void onIncreaseFinished();
    
    bool isIncreaseFinished();
    
protected:
    OnIncreaseListener onIncreaseListener;
    int startNumber;        // 시작 숫자
    int targetNumber;       // 목표 숫자
    int number;             // 현재 숫자
    int increasePerStep;    // 단계 당 증가할 숫자
};

#endif /* SBAction_hpp */
