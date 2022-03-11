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

/**
 * 캐릭터 이미지를 반환합니다
 */
string ResourceHelper::getCharacterImage(const string &charId) {
    return DIR_CONTENT_CHARACTER + STR_FORMAT("%s.png", charId.c_str());
}

/**
 * 월드 배경 이미지를 반환합니다
 */
string ResourceHelper::getWorldBackgroundImage(int world) {
    return DIR_CONTENT_BG + STR_FORMAT("world_%02d.png", world);
}

/**
 * 타일 이미지를 반환합니다
 */
StringList ResourceHelper::getTileImage(const TileId &tileId) {
    
    StringList files;
    
    // 단일 이미지
    auto file = DIR_CONTENT_TILE + STR_FORMAT("tile_%05d.png", (int)tileId);
    
    if( FileUtils::getInstance()->isFileExist(file) ) {
        files.push_back(file);
    }
    // 애니메이션
    else {
        int i = 0;
        
        while( true ) {
            auto file = DIR_CONTENT_TILE + STR_FORMAT("tile_%05d_%d.png", (int)tileId, i+1);
            
            if( !FileUtils::getInstance()->isFileExist(file) ) {
                break;
            }
            
            files.push_back(file);
            i++;
        }
    }
    
    return files;
}

/**
 * 타일 스파인 json 파일을 반환합니다
 */
string ResourceHelper::getTileSkeletonJsonFile(const TileId &tileId) {
    
    auto file = DIR_CONTENT_TILE + STR_FORMAT("tile_%05d.json", (int)tileId);
    
    if( FileUtils::getInstance()->isFileExist(file) ) {
        return file;
    }
    
    return "";
}




