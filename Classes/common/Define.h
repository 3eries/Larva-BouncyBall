//
//  Define.h
//  Define
//
//  Created by seongmin hwang
//

#ifndef Define_h
#define Define_h

#include "cocos2d.h"
#include "superbomb.h"

static const std::string DEV_VERSION = "v0.0.0.1";

// Common Functions
void removeListeners(cocos2d::Ref *target);

// Config File
static const std::string SERVER_VERSION_FILE                    = "http://superbomb.co/3eries/rsp/version.json";
static const std::string SERVER_GAME_CONFIG_FILE                = "http://superbomb.co/3eries/rsp/game_config.json";

#define                  AES256_KEY                               "A1din7aah98dh9wef974fnsudh54ugJs"

static const std::string GAME_CONFIG_FILE                       = "game_config.json";
static const std::string PLUGIN_CONFIG_FILE                     = "config/plugin_config.json";
static const std::string CHARACTER_CONFIG_FILE                  = "content/character/character.json";

static const std::string INTERNAL_GAME_CONFIG_FILE              = "config/" + GAME_CONFIG_FILE;
#define                  EXTERNAL_GAME_CONFIG_FILE                 std::string(cocos2d::FileUtils::getInstance()->getWritablePath() + GAME_CONFIG_FILE)

static const std::string INTERNAL_GAME_CONFIG_FILE_VERSION      = "1.0.0";

#define                  ENCRYPT_GAME_CONFIG_FILE                 0
#define                  ENCRYPT_PLUGIN_CONFIG_FILE               0
#define                  ENCRYPT_CHARACTER_CONFIG_FILE            0

// Analytics
static const std::string ANALYTICS_SCREEN_SPLASH                    = "splash";
static const std::string ANALYTICS_SCREEN_WELCOME                   = "welcome";
static const std::string ANALYTICS_SCREEN_MAIN                      = "main";
static const std::string ANALYTICS_SCREEN_GAME                      = "game";

static const std::string ANALYTICS_EVENT_CREDIT                     = "credit";                     // 크레딧 화면 진입

static const std::string ANALYTICS_EVENT_STAGE_PLAY                 = "stage_play";
static const std::string ANALYTICS_EVENT_STAGE_CLEAR                = "stage_clear";
static const std::string ANALYTICS_EVENT_STAGE_PLAY_RE              = "stage_play_re";
static const std::string ANALYTICS_EVENT_STAGE_CLEAR_RE             = "stage_clear_re";

static const std::string ANALYTICS_EVENT_GAME_OVER                  = "game_over";

static const std::string ANALYTICS_EVENT_PARAM_SUMMARY              = "summary";                    // 요약
static const std::string ANALYTICS_EVENT_PARAM_STAGE                = "stage";
static const std::string ANALYTICS_EVENT_PARAM_STAGE_RANGE          = "stage_range";
static const std::string ANALYTICS_EVENT_PARAM_STAR                 = "star";
static const std::string ANALYTICS_EVENT_PARAM_TYPE                 = "type";

// Leaderboard
static const std::string LEADER_BOARD_TOP_LEVEL                     = "CgkI3oP_ia0VEAIQAQ";

// cocos2d::Director Event
static const std::string DIRECTOR_EVENT_REMOVE_ADS              = "DIRECTOR_EVENT_REMOVE_ADS";
static const std::string DIRECTOR_EVENT_UPDATE_HINT_COUNT       = "DIRECTOR_EVENT_UPDATE_HINT_COUNT";
static const std::string DIRECTOR_EVENT_UPDATE_USER_COIN        = "DIRECTOR_EVENT_UPDATE_USER_COIN";
static const std::string DIRECTOR_EVENT_UNLOCK_ALL_PUZZLES      = "DIRECTOR_EVENT_UNLOCK_ALL_PUZZLES";

// 리소스 경로
static const std::string DIR_IMG                    = "images/";
static const std::string DIR_IMG_COMMON             = DIR_ADD(DIR_IMG, "common");
static const std::string DIR_IMG_SPLASH             = DIR_ADD(DIR_IMG, "splash");
static const std::string DIR_IMG_MAIN               = DIR_ADD(DIR_IMG, "main");
static const std::string DIR_IMG_MAIN_IPAD          = DIR_ADD(DIR_IMG_MAIN, "ipad");
static const std::string DIR_IMG_GAME               = DIR_ADD(DIR_IMG, "game");
static const std::string DIR_IMG_GAME_IPAD          = DIR_ADD(DIR_IMG_GAME, "ipad");
static const std::string DIR_IMG_RESULT             = DIR_ADD(DIR_IMG, "result");
static const std::string DIR_IMG_SHOP               = DIR_ADD(DIR_IMG, "shop");
static const std::string DIR_IMG_EXIT               = DIR_ADD(DIR_IMG, "exit");
static const std::string DIR_IMG_SETTING            = DIR_ADD(DIR_IMG, "setting");
static const std::string DIR_IMG_PAUSE              = DIR_ADD(DIR_IMG, "pause");
static const std::string DIR_IMG_UPDATE             = DIR_ADD(DIR_IMG, "update");

static const std::string DIR_CONTENT                = "content/";
static const std::string DIR_CONTENT_CHARACTER      = DIR_ADD(DIR_CONTENT, "character");
static const std::string DIR_CONTENT_BG             = DIR_ADD(DIR_CONTENT, "bg");
static const std::string DIR_CONTENT_STAGE          = DIR_ADD(DIR_CONTENT, "stage");
static const std::string DIR_CONTENT_TILE           = DIR_ADD(DIR_CONTENT, "tile");

// 스파인 애니메이션
static const std::string ANIM_EXT                   = ".json";
static const std::string ANIM_LOGO                  = DIR_IMG_SPLASH + "3eries_logo.json";

static const std::string ANIM_NAME_RUN              = "run";
static const std::string ANIM_NAME_CLEAR            = "clear";
static const std::string ANIM_NAME_ENTER            = "enter";
static const std::string ANIM_NAME_EXIT             = "exit";

// 폰트 경로
static const std::string DIR_FONT                   = "fonts/";

static const std::string FONT_COMMODORE             = DIR_FONT + "Commodore.ttf";
static const std::string FONT_SABO                  = DIR_FONT + "Sabo-Filled.otf";
static const std::string FONT_MANIA                 = DIR_FONT + "mania.ttf";
static const std::string FONT_GAME_OVER             = DIR_FONT + "game_over.ttf";
static const std::string FONT_RETRO                 = DIR_FONT + "Commodore.ttf";
static const std::string FONT_NEW_RECORD            = DIR_FONT + "Sabo-Filled.otf";
static const std::string FONT_SUPER_STAR            = DIR_FONT + "superstar_memesbruh03.ttf";
static const std::string FONT_ROBOTO_BLACK          = DIR_FONT + "Roboto-Black.ttf";

// 사운드 경로
static const std::string DIR_SOUND                  = "sounds/";

static const std::string SOUND_BGM_MAIN             = DIR_SOUND + "";
static const std::string SOUND_BGM_GAME             = DIR_SOUND + "";

static const std::string SOUND_BUTTON_CLICK         = DIR_SOUND + "effect_01.mp3";
static const std::string SOUND_JUMP                 = DIR_SOUND + "effect_01.mp3";
static const std::string SOUND_LEVEL_CLEAR          = DIR_SOUND + "effect_05.mp3";

// 좌표 및 크기
#define BANNER_HEIGHT           superbomb::AdsHelper::getBannerHeight()

// Color
namespace Color {
    static const cocos2d::Color4B POPUP_BG          = cocos2d::Color4B(0,0,0,255*0.5f);
}

// ZOrder
namespace ZOrder {
    static const int SCENE_TOUCH_LOCKED        = SBZOrder::MIDDLE-1;
    
    static const int POPUP_BOTTOM              = SBZOrder::MIDDLE;
    static const int POPUP_MIDDLE              = POPUP_BOTTOM + 10;
    static const int POPUP_TOP                 = POPUP_MIDDLE + 10;
    
    static const int COMMON_MENU_TOP           = POPUP_TOP;
    static const int COMMON_MENU_BOTTOM        = POPUP_MIDDLE;
}

// 연출 시간
namespace EffectDuration {
    static const float POPUP_SLIDE_SLOW        = 0.35f;
    static const float POPUP_SLIDE_NORMAL      = 0.25f;
    static const float POPUP_SLIDE_FAST        = 0.2f;
    
    static const float MENU_SLIDE_NORMAL       = 0.3f;
    static const float MENU_SLIDE_FAST         = 0.25f;
}

// UI
namespace ButtonZoomScale {
    static const float WEAK                    = -0.03f;
    static const float NORMAL                  = -0.05f;
    static const float HARD                    = -0.07f;
}

#endif /* Define_h */
