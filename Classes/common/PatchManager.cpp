//
//  PatchManager.cpp
//
//  Created by seongmin hwang on 2018. 6. 6..
//

#include "PatchManager.hpp"

#include "Define.h"
#include "UserDefaultKey.h"
#include "GameConfiguration.hpp"

USING_NS_CC;
using namespace std;

static PatchManager *instance = nullptr;
PatchManager* PatchManager::getInstance() {
    
    if( !instance ) {
        instance = new PatchManager();
    }
    
    return instance;
}

void PatchManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

PatchManager::PatchManager() :
onAppVersion(nullptr),
onGameConfig(nullptr) {
    
}

PatchManager::~PatchManager() {
    
}

void PatchManager::init() {
    
    // 게임 설정 파일 복사
    copyConfigFile();
}

void PatchManager::patch() {
    
    // config
    {
        string json = SBStringUtils::readTextFile(INTERNAL_GAME_CONFIG_FILE);
#if ENCRYPT_GAME_CONFIG_FILE
        json = SBSecurity::decryptAES256(json, AES256_KEY);
#endif
        GameConfiguration::getInstance()->parse(json);
    }
    
    // TODO:
    // 1. 서버의 버전 파일 읽어옴 -> onAppVersion
    // 2. 서버의 게임 설정 파일 읽어옴 -> onGameConfig
    // ? 내부에서 GameConfiguration에 접근할 것인가?
    /*
    auto request = new SBHttpRequest();
    request->setUrl(SERVER_VERSION_FILE);
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setOnSucceedListener([=](network::HttpResponse *res, string result) {
        
        CCLOG("get version.json: %s", result.c_str());
    });
    request->setOnErrorListener([=](network::HttpResponse *res) {
        
        CCLOG("get version.json error: %s", res->getErrorBuffer());
    });
    request->send();
    request->release();
     */
}

/**
 * 게임 설정 파일(game_config.json) 복사
 * 앱 실행 후 최초 한 번만 수행
 * internal -> external
 */
void PatchManager::copyConfigFile() {
    
    CCLOG("EXTERNAL_GAME_CONFIG_FILE: %s", EXTERNAL_GAME_CONFIG_FILE.c_str());
    CCLOG("getExternalConfigFileVersion: %s", getExternalConfigFileVersion().c_str());
    CCLOG("INTERNAL_GAME_CONFIG_FILE_VERSION: %s", INTERNAL_GAME_CONFIG_FILE_VERSION.c_str());
    
    // 이미 복사됬는지 체크
    if( FileUtils::getInstance()->isFileExist(EXTERNAL_GAME_CONFIG_FILE) ) {
        if( getExternalConfigFileVersion() >= INTERNAL_GAME_CONFIG_FILE_VERSION ) {
            // 버전 체크
            return;
        }
    }
    
    string str = SBStringUtils::readTextFile(INTERNAL_GAME_CONFIG_FILE);
    SBStringUtils::writeTextFile(EXTERNAL_GAME_CONFIG_FILE, str);
    
    setExternalConfigFileVersion(INTERNAL_GAME_CONFIG_FILE_VERSION);
}

void PatchManager::setExternalConfigFileVersion(string version) {
    
    UserDefault::getInstance()->setStringForKey(UserDefaultKey::EXTERNAL_GAME_CONFIG_FILE_VERSION, version);
    UserDefault::getInstance()->flush();
}

string PatchManager::getExternalConfigFileVersion() {
    
    return UserDefault::getInstance()->getStringForKey(UserDefaultKey::EXTERNAL_GAME_CONFIG_FILE_VERSION, "");
}

