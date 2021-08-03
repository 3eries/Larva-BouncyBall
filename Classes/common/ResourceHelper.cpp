//
//  ResourceHelper.cpp
//
//  Created by seongmin hwang on 2018. 5. 17..
//

#include "ResourceHelper.hpp"

#include "Define.h"

USING_NS_CC;
using namespace std;

void ResourceHelper::preload() {
    
    // image
    auto textureCache = Director::getInstance()->getTextureCache();
    
    auto getAnimPNG = [](string anim) -> string {
        return SBStringUtils::replaceAll(anim, ANIM_EXT, ".png");
    };
    
    auto addImageAsync = [textureCache](StringList list) {
      
        for( auto file : list ) {
            textureCache->addImageAsync(file, nullptr);
        }
    };
    
    // add image
    {
        string files[] = {
            // spine resource
        };
        
        for( string file : files ) {
            textureCache->addImage(getAnimPNG(file));
        }
    }
    
    // add image async
    {
        string files[] = {
            // spine resource
        };
        
        for( string file : files ) {
            textureCache->addImageAsync(getAnimPNG(file), nullptr);
        }
        
        // ...2.png
        {
            string files[] = {
                // spine resource
            };
            
            for( string file : files ) {
                const string secondFile = SBStringUtils::replaceAll(getAnimPNG(file), ".png", "2.png");
                textureCache->addImageAsync(getAnimPNG(secondFile), nullptr);
            }
        }
    }
    
    // sound
    SBAudioEngine::preload(SOUND_BGM_MAIN);
    SBAudioEngine::preload(SOUND_BGM_GAME);
    SBAudioEngine::preload(SOUND_BUTTON_CLICK);
}

string ResourceHelper::getLocalizedImage(const string &image) {
    
    LanguageType lanType = Application::getInstance()->getCurrentLanguage();
    string lanCode = "en";
    
    switch( lanType ) {
        case LanguageType::KOREAN:      lanCode = "ko";     break;
        default: break;
    }
    
    return SBStringUtils::replaceAll(image, ".png", STR_FORMAT("_%s.png", lanCode.c_str()));
}
