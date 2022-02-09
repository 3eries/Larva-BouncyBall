//
//  GameDefine.h
//
//  Created by seongmin hwang on 19/08/2019.
//

#ifndef GameDefine_h
#define GameDefine_h

#include "cocos2d.h"
#include "superbomb.h"

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

typedef std::vector<GameEvent> GameEventList;

static std::map<GameEvent, std::string> GAME_EVENT_STRINGS = {
    { GameEvent::ENTER,             "GAME_EVENT_ENTER" },
    { GameEvent::EXIT,              "GAME_EVENT_EXIT" },
    { GameEvent::RESET,             "GAME_EVENT_RESET" },
    { GameEvent::START,             "GAME_EVENT_START" },
    { GameEvent::RESTART,           "GAME_EVENT_RESTART" },
    { GameEvent::PAUSE,             "GAME_EVENT_PAUSE" },
    { GameEvent::RESUME,            "GAME_EVENT_RESUME" },
    { GameEvent::OVER,              "GAME_EVENT_OVER" },
    { GameEvent::CONTINUE,          "GAME_EVENT_CONTINUE" },
    { GameEvent::RESULT,            "GAME_EVENT_RESULT" },
    { GameEvent::STAGE_CHANGED,     "GAME_EVENT_STAGE_CHANGED" },
    { GameEvent::STAGE_RESTART,     "GAME_EVENT_STAGE_RESTART" },
    { GameEvent::STAGE_CLEAR,       "GAME_EVENT_STAGE_CLEAR" },
    { GameEvent::MOVE_NEXT_STAGE,   "GAME_EVENT_MOVE_NEXT_STAGE" },
    { GameEvent::MOVE_NEXT_STAGE_FINISHED, "GAME_EVENT_MOVE_NEXT_STAGE_FINISHED" },
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

// In Game
enum class BallDirection {
    NONE,
    LEFT,
    RIGHT,
};

#define                 GAME_BG_COLOR                   cocos2d::Color3B(0, 17, 17)
#define                 LEVEL_REFRESH_DURATION          1.7f
#define                 MOVE_NEXT_LEVEL_DURATION        0.5f

// Physics
#define                PHYSICS_FPS                        (1 / 60.0f)
#define                PHYSICS_GRAVITY                    b2Vec2(0, -60 * GAME_MANAGER->getMapScaleFactor())

// velocityIterations : 바디들을 정상적으로 이동시키기 위해서 필요한 충돌들을 반복적으로 계산
// positionIterations : 조인트 분리와, 겹침현상을 줄이기 위해서 바디의 위치를 반복적으로 적용
// 값이 클수록 정확한 연산이 가능하지만 성능이 떨어진다.
// 매뉴얼상의 권장값 (VELOCITY_ITERATIONS=8, POSITION_ITERATIONS=3)
#define                VELOCITY_ITERATIONS                8
#define                POSITION_ITERATIONS                4

// 천장 - ceiling

enum PhysicsCategory {
    WALL_LEFT    = (1 << 0),     // 왼쪽 벽
    WALL_RIGHT   = (1 << 1),     // 오른쪽 벽
    WALL_TOP     = (1 << 2),     // 천장
    FLOOR        = (1 << 3),     // 바닥
    BALL         = (1 << 4),
    FLAG         = (1 << 5),
    ITEM         = (1 << 6),
    BLOCK_TOP    = (1 << 7),     // 블럭 상단
    BLOCK_SIDE   = (1 << 8),     // 블럭 측면
    BLOCK_BOTTOM = (1 << 9),     // 블럭 하단
};

static const uint16 PHYSICS_MASK_BITS_WALL = (PhysicsCategory::BALL);
static const uint16 PHYSICS_MASK_BITS_BALL = (PhysicsCategory::WALL_LEFT | PhysicsCategory::WALL_RIGHT | PhysicsCategory::WALL_TOP | PhysicsCategory::FLOOR | PhysicsCategory::FLAG | PhysicsCategory::ITEM | PhysicsCategory::BLOCK_TOP | PhysicsCategory::BLOCK_SIDE | PhysicsCategory::BLOCK_BOTTOM);

#define                 TILE_DEFAULT_SIZE                  cocos2d::Size(108,108)

#define                 TILE_PADDING_X                     4
#define                 TILE_PADDING_Y                     4

#define MEASURE_TILE_SIZE(__rows__, __columns__) \
cocos2d::Size((TILE_CONTENT_SIZE.width*__rows__) + (TILE_PADDING_X*(__rows__-1)), (TILE_CONTENT_SIZE.height*__columns__) + (TILE_PADDING_Y*(__columns__-1)))

//static inline cocos2d::Size getTileContentSize(int rows, int columns) {
//
//    cocos2d::Size size;
//    size.width += TILE_CONTENT_SIZE.width * columns;
//    size.width += TILE_PADDING * (columns-1);
//    size.height += TILE_CONTENT_SIZE.height * rows;
//    size.height += TILE_PADDING * (rows-1);
//
//    return size;
//}

//static inline cocos2d::Vec2 convertToTilePosition(int x, int y, int w, int h) {
//
//    cocos2d::Vec2 pos(TILE_ORIGIN);
//
//    // content size
//    pos.x += x * TILE_CONTENT_WIDTH;
//    pos.y += y * TILE_CONTENT_HEIGHT;
//    // padding
//    pos.x += x * TILE_PADDING_X;
//    pos.y += y * TILE_PADDING_Y;
//    // anchor middle
//    cocos2d::Size SIZE = MEASURE_TILE_SIZE(w,h);
//    pos.x += SIZE.width * 0.5f;
//    pos.y += SIZE.height * 0.5f;
//
//    return pos;
//}

#endif /* GameDefine_h */
