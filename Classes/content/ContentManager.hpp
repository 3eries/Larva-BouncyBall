//
//  ContentManager.hpp
//
//  Created by seongmin hwang on 07/02/2019.
//

#ifndef ContentManager_hpp
#define ContentManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "data/Database.hpp"
#include "ContentResourceHelper.hpp"

/** @class ContentManager
 * @brief 이 클래스는 컨텐츠를 관리합니다
 */
class ContentManager {
public:
    static ContentManager* getInstance();
    static void destroyInstance();
    ~ContentManager();
    
    void init();
    
private:
    ContentManager();
    
public:
    static Database* getDatabase();
    
private:
    Database *db;
};

#endif /* ContentManager_hpp */
