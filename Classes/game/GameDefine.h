//
//  GameDefine.h
//
//  Created by seongmin hwang on 19/08/2019.
//

#ifndef GameDefine_h
#define GameDefine_h

#include "cocos2d.h"
#include "superbomb.h"

static const std::string GAME_EVENT_ENTER                         = "GAME_EVENT_ENTER";
static const std::string GAME_EVENT_EXIT                          = "GAME_EVENT_EXIT";
static const std::string GAME_EVENT_RESET                         = "GAME_EVENT_RESET";
static const std::string GAME_EVENT_START                         = "GAME_EVENT_START";
static const std::string GAME_EVENT_RESTART                       = "GAME_EVENT_RESTART";
static const std::string GAME_EVENT_PAUSE                         = "GAME_EVENT_PAUSE";
static const std::string GAME_EVENT_RESUME                        = "GAME_EVENT_RESUME";
static const std::string GAME_EVENT_OVER                          = "GAME_EVENT_OVER";
static const std::string GAME_EVENT_CONTINUE                      = "GAME_EVENT_CONTINUE";
static const std::string GAME_EVENT_RESULT                        = "GAME_EVENT_RESULT";
static const std::string GAME_EVENT_STAGE_CHANGED                 = "GAME_EVENT_STAGE_CHANGED";
static const std::string GAME_EVENT_STAGE_RESTART                 = "GAME_EVENT_STAGE_RESTART";
static const std::string GAME_EVENT_STAGE_CLEAR                   = "GAME_EVENT_STAGE_CLEAR";
static const std::string GAME_EVENT_MOVE_NEXT_STAGE               = "GAME_EVENT_MOVE_NEXT_STAGE";
static const std::string GAME_EVENT_MOVE_NEXT_STAGE_FINISHED      = "GAME_EVENT_MOVE_NEXT_STAGE_FINISHED";

enum class GameEvent {
    NONE = 0,
    ENTER = 1,
    EXIT,
    RESET,
    START,
    RESTART,
    PAUSE,
    RESUME,
    OVER,
    CONTINUE,
    RESULT,
    STAGE_CHANGED,
    STAGE_RESTART,
    STAGE_CLEAR,
    MOVE_NEXT_STAGE,
    MOVE_NEXT_STAGE_FINISHED,
};

static std::map<std::string, GameEvent> GAME_EVENT_ENUMS = {
    { GAME_EVENT_ENTER, GameEvent::ENTER },
    { GAME_EVENT_EXIT, GameEvent::EXIT },
    { GAME_EVENT_RESET, GameEvent::RESET },
    { GAME_EVENT_START, GameEvent::START },
    { GAME_EVENT_RESTART, GameEvent::RESTART },
    { GAME_EVENT_PAUSE, GameEvent::PAUSE },
    { GAME_EVENT_RESUME, GameEvent::RESUME },
    { GAME_EVENT_OVER, GameEvent::OVER },
    { GAME_EVENT_CONTINUE, GameEvent::CONTINUE },
    { GAME_EVENT_RESULT, GameEvent::RESULT },
    { GAME_EVENT_STAGE_CHANGED, GameEvent::STAGE_CHANGED },
    { GAME_EVENT_STAGE_RESTART, GameEvent::STAGE_RESTART },
    { GAME_EVENT_STAGE_CLEAR, GameEvent::STAGE_CLEAR },
    { GAME_EVENT_MOVE_NEXT_STAGE, GameEvent::MOVE_NEXT_STAGE },
    { GAME_EVENT_MOVE_NEXT_STAGE_FINISHED, GameEvent::MOVE_NEXT_STAGE_FINISHED },
};

enum GameState {
    NONE            = (1 << 0),
    ENTERED         = (1 << 1),     // 게임 진입
    EXITED          = (1 << 2),     // 게임 퇴장
    STARTED         = (1 << 3),     // 게임 시작됨
    PAUSED          = (1 << 4),     // 일시정지
    IDLE            = (1 << 5),     // 대기
    GAME_OVER       = (1 << 6),     // 게임 오버
    CONTINUE        = (1 << 7),     // 이어하기
    RESULT          = (1 << 8),     // 결과 화면
};

#define                 GAME_BG_COLOR                   cocos2d::Color3B(0, 17, 17)
#define                 LEVEL_REFRESH_DURATION          1.7f
#define                 MOVE_NEXT_LEVEL_DURATION        0.5f

//////////// Tile
static const std::vector<cocos2d::Color3B> TILE_COLORS({
    cocos2d::Color3B(239, 255, 233),
    cocos2d::Color3B(46, 196, 182),
    cocos2d::Color3B(231, 40, 59),
    cocos2d::Color3B(246, 234, 140),
    cocos2d::Color3B(188, 205, 244),
    cocos2d::Color3B(252,145, 58),
    cocos2d::Color3B(136, 219, 163),
    cocos2d::Color3B(37, 128, 228),
    cocos2d::Color3B(225, 59, 106),
    cocos2d::Color3B(110, 107, 107),
    cocos2d::Color3B(25, 144, 144),
    cocos2d::Color3B(188, 255, 255),
    cocos2d::Color3B(131, 87, 208),
});

#define                 TILE_MAP_CONTENT_SIZE               cocos2d::Size(648,1024)
#define                 TILE_CONTENT_SIZE                   cocos2d::Size(84,84)
#define                 TILE_PADDING                        10

#define                 TILE_SELECTED_LINE_SIZE             6
#define                 TILE_SELECTED_LINE_COLOR            cocos2d::Color3B(0, 255, 36)
#define                 TILE_HINT_LINE_COLOR                cocos2d::Color3B(255, 0, 25)

#define                 TILE_NUMBER_NORMAL_COLOR            cocos2d::Color4B(0,0,0,255)
#define                 TILE_NUMBER_SELECTED_COLOR          cocos2d::Color4B(255,255,255,255)

#define                 TILE_ENTER_DURATION                 0.3f
#define                 TILE_EXIT_DURATION                  0.3f
#define                 TILE_MOVE_DURATION                  0.3f

#define                 TILE_NUMBER_ENTER_DURATION          0.3f
#define                 TILE_NUMBER_EXIT_DURATION           0.3f

static inline cocos2d::Size getTileContentSize(int rows, int columns) {
    
    cocos2d::Size size;
    size.width += TILE_CONTENT_SIZE.width * columns;
    size.width += TILE_PADDING * (columns-1);
    size.height += TILE_CONTENT_SIZE.height * rows;
    size.height += TILE_PADDING * (rows-1);
    
    return size;
}

static inline cocos2d::Vec2 convertTilePosition(int x, int y) {
    
    cocos2d::Vec2 pos;
    pos.x += TILE_CONTENT_SIZE.width * x;
    pos.y += TILE_CONTENT_SIZE.height * y;
    if( x > 0 ) pos.x += TILE_PADDING * x;
    if( y > 0 ) pos.y += TILE_PADDING * y;
    
    // 타일 중점
    pos.x += TILE_CONTENT_SIZE.width * 0.5f;
    pos.y += TILE_CONTENT_SIZE.height * 0.5f;
    
    return pos;
}

static inline cocos2d::Vec2 convertTilePosition(const cocos2d::Vec2 &p) {
    return convertTilePosition((int)p.x, (int)p.y);
}

#endif /* GameDefine_h */
