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
#include "StageManager.hpp"

#define LOCALIZED_IMAGE(__image__) \
ResourceHelper::getLocalizedImage(__image__)

class ResourceHelper {
public:
    static void preload();
    static std::string getLocalizedImage(const std::string &image);
    
    static std::string getCharacterImage(const std::string &charId);
    
    static std::string getWorldBackgroundMusic(int world);
    static std::string getWorldBackgroundImage(int world);
    
    static StringList  getTileImage(const TileId &tileId);
    static std::string getTileSkeletonJsonFile(const TileId &tileId);
};

#endif /* ResourceHelper_hpp */
