//
//  ResourceHelper.hpp
//
//  Created by seongmin hwang on 2018. 5. 17..
//

#ifndef ResourceHelper_hpp
#define ResourceHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#define LOCALIZED_IMAGE(__image__) \
ResourceHelper::getLocalizedImage(__image__)

class ResourceHelper {
public:
    static void preload();
    
    static std::string getLocalizedImage(const std::string &image);
};

#endif /* ResourceHelper_hpp */
