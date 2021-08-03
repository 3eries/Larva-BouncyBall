//
//  SBMath.cpp
//
//  Created by seongmin hwang on 2018. 9. 18..
//

#include "SBMath.hpp"

USING_NS_CC;
using namespace std;

/**
 * 자릿수를 반환합니다
 */
int SBMath::getDigit(int n) {
    
    if( n < 0 )
        n = -n;
    
    int digit = 0;
    
    for( ; n > 0; n /= 10, digit++ );
    
    return digit;
}

/**
 * 두 점 사이의 각도를 반환합니다
 */
float SBMath::getDegree(const Vec2 &from, const Vec2 &to) {
    
    /*
    float d = -CC_RADIANS_TO_DEGREES(atan2f(to.y - from.y, to.x - from.x)) + 90;
    return d < 0 ? d+360 : d;
    */
     
    return CC_RADIANS_TO_DEGREES(atan2f(to.x - from.x, to.y - from.y));
}

/**
 * 종료 좌표를 반환합니다
 */
Vec2 SBMath::getEndPosition(const Vec2 &startPos, float angle, float dist) {
    
    Vec2 p = startPos;
    p += (Vec2::forAngle(-CC_DEGREES_TO_RADIANS(angle-90)) * dist);
    
    return p;
}

int SBMath::random(int min, int max) {
 
    random_device rd;
    mt19937 engine(rd());
    
    uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}
