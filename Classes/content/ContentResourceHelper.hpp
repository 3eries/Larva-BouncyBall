//
//  ContentResourceHelper.hpp
//
//  Created by seongmin hwang on 07/02/2019.
//

#ifndef ContentResourceHelper_hpp
#define ContentResourceHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

class ContentResourceHelper {
public:
    static ContentResourceHelper* getInstance();
    static void destroyInstance();
    ~ContentResourceHelper();
    
private:
    ContentResourceHelper();
    
public:
    static void preload();
};

#endif /* ContentResourceHelper_hpp */
