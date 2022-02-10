//
//  GameView.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameView.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "GameUIHelper.hpp"
#include "ContentManager.hpp"
#include "SceneManager.h"

#include "object/Ball.hpp"
#include "object/tile/Flag.hpp"
#include "object/tile/ClearPortal.hpp"
#include "object/tile/Item.hpp"
#include "object/tile/Block.hpp"

#include "object/StageProgressBar.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define SCHEDULER_UPDATE_CAMERA             "UPDATE_CAMERA"
#define TOUCH_TAP_DURATION                  0.1f            // 터치 탭 판정 시간

#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG == 1
#define DEBUG_DRAW_PHYSICS                  1
#else
#define DEBUG_DRAW_PHYSICS                  0
#endif // COCOS2D_DEBUG == 1

GameView::GameView(): SBPhysicsObject(this),
isTouchEnabled(false) {
}

GameView::~GameView() {
}

bool GameView::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(ANCHOR_M);
    setPosition(Vec2MC(0,0));
    setContentSize(SB_WIN_SIZE);
    
    initPhysics();
    initCamera();
    initBg();
    initTiles();
    initBall();
    initGameListener();
    initTouchListener();
    
    return true;
}

void GameView::onEnter() {
    
    Log::i("GameView::onEnter");
    
    Node::onEnter();
    
    onStageChanged(GAME_MANAGER->getStage());
}

void GameView::onEnterTransitionDidFinish() {
    
    Node::onEnterTransitionDidFinish();
    
    // 튜토리얼
    if( GAME_MANAGER->getStage().stage == 1 ) {
    }
    
    // 카메라 업데이트 스케줄러 실행
    schedule(CC_CALLBACK_1(GameView::updateCamera, this), SCHEDULER_UPDATE_CAMERA);
    
    // update info label
#if DEBUG_DRAW_PHYSICS
    schedule([=](float dt) {
        auto body = ball->getBody();
        auto velocity = body->GetLinearVelocity();
        
        // CCLOG("velocity: %f, %f", velocity.x, velocity.y);
        this->maxVelocity.x = MAX(velocity.x, this->maxVelocity.x);
        this->maxVelocity.y = MAX(velocity.y, this->maxVelocity.y);
        this->minVelocity.x = MIN(velocity.x, this->minVelocity.x);
        this->minVelocity.y = MIN(velocity.y, this->minVelocity.y);
        
        string msg = "";
        msg += STR_FORMAT("x: %d", (int)velocity.x) + "\n";
        msg += STR_FORMAT("y: %d", (int)velocity.y) + "\n";
        msg += STR_FORMAT("maxX: %d", (int)maxVelocity.x) + "\n";
        msg += STR_FORMAT("maxY: %d", (int)maxVelocity.y) + "\n";
        msg += STR_FORMAT("minX: %d", (int)minVelocity.x) + "\n";
        msg += STR_FORMAT("minY: %d", (int)minVelocity.y) + "\n";
        
        infoLabel->setString(msg);
        
    }, 0.1f, "TEST");
#endif
}

void GameView::cleanup() {
    
    removeListeners(this);
    
    Node::cleanup();
}

#pragma mark- Game Event

/**
 * 게임 리셋
 */
void GameView::onGameReset() {
}

/**
 * 게임 종료
 */
void GameView::onGameExit() {
    
#if DEBUG_DRAW_PHYSICS
    removeChildByTag(Tag::DEBUG_DRAW_VIEW);
#endif
    
    SBNodeUtils::recursivePauseSchedulerAndActions(this);
}

/**
 * 게임 일시정지
 */
void GameView::onGamePause() {
    
    getScheduler()->pauseTarget(this);
    getActionManager()->pauseTarget(this);
}

/**
 * 게임 재개
 */
void GameView::onGameResume() {
    
    getScheduler()->resumeTarget(this);
    getActionManager()->resumeTarget(this);
}

/**
 * 스테이지 변경
 */
void GameView::onStageChanged(const StageData &stage) {
    
    isTouchEnabled = true;
}

/**
 * 스테이지 재시작
 */
void GameView::onStageRestart(const StageData &stage) {
}

/**
 * 스테이지 클리어
 */
void GameView::onStageClear(const StageData &stage) {
}

/**
 * 다음 스테이지로 이동
 */
void GameView::onMoveNextStage() {
    
    // 연출 완료
    SBDirector::postDelayed(this, [=]() {
        GameManager::onMoveNextStageFinished();
    }, MOVE_NEXT_LEVEL_DURATION, true);
}

/**
 * 다음 스테이지로 이동 완료
 */
void GameView::onMoveNextStageFinished() {
}

/**
 * 카메라 업데이트
 */
void GameView::updateCamera(float dt) {
    
    auto MAP_CONTENT_SIZE = GAME_MANAGER->getStage().mapContentSize;
    
    if( MAP_CONTENT_SIZE.width <= SB_WIN_SIZE.width ) {
        return;
    }
    
    auto chcBox = SB_BOUNDING_BOX_IN_WORLD(ball);
    
    // 캐릭터 좌표 체크
    float chcDiff = (SB_WIN_SIZE.width*0.5f) - chcBox.getMidX();
    // CCLOG("chcDiff: %f", chcDiff);
    
    // 캐릭터가 화면 가운데로 위치하도록 캠 노드 좌표 변경
    Vec3 pos = mapCameraInfo.camControlNode->getPosition3D();
    pos.x = SB_WIN_SIZE.width*0.5f; // reset
    pos.x -= chcDiff;
    pos.x = MAX(SB_WIN_SIZE.width*0.5f, pos.x);
    pos.x = MIN(MAP_CONTENT_SIZE.width - (SB_WIN_SIZE.width*0.5f), pos.x);
    mapCameraInfo.camControlNode->setPosition3D(pos);

    // 카메라 이동
    Vec3 worldPos;
    mapCameraInfo.camNode->getNodeToWorldTransform().getTranslation(&worldPos);

    mapCameraInfo.camera->setPosition3D(worldPos);
    mapCameraInfo.camera->lookAt(mapCameraInfo.camControlNode->getPosition3D());
}

/**
 * 타일을 추가합니다
 */
void GameView::addTile(GameTile *tile) {
    
    tile->setCameraMask((unsigned short)CAMERA_FLAG_MAP);
    
    int z = 0;
    
    switch( tile->getData().type ) {
        case TileType::FLAG:    z = ZOrder::TILE_FLAG;      break;
        case TileType::ITEM:    z = ZOrder::TILE_ITEM;      break;
        case TileType::BLOCK:   z = ZOrder::TILE_BLOCK;      break;
        default: break;
    }
    
    addChild(tile, z);
    tiles.push_back(tile);
}

/**
 * 타일을 제거합니다
 */
void GameView::removeTile(GameTile *tile) {
 
    tile->prepareRemove();
    tile->setLazyRemove(true);
    tile->setVisible(false);
    
    SBCollection::remove(tiles, tile);
}

/**
 * 좌표에 해당하는 타일을 반환합니다
 */
GameTile* GameView::getTile(const TilePosition &p) {
    
    vector<GameTile*> foundTiles = SBCollection::find(tiles, [=](GameTile *tile) -> bool {
        return tile->getData().p == p;
    });
    CCASSERT(foundTiles.size() <= 1, "GameView::getTile error.");
    
    return foundTiles[0];
}

/**
 * 타입에 해당하는 타일을 반환합니다
 */
vector<GameTile*> GameView::getTiles(const TileId &tileId) {
    
    vector<GameTile*> foundTiles = SBCollection::find(tiles, [=](GameTile *tile) -> bool {
        return tile->getData().tileId == tileId;
    });
    return foundTiles;
}

#pragma mark- Touch Event

void GameView::onTouchBegan(Touch *touch) {
    
    bool isLeft = (touch->getLocation().x < SB_WIN_SIZE.width*0.5f);

    if( isLeft ) {
        ball->moveLeft();
    } else {
        ball->moveRight();
    }
}

void GameView::onTouchEnded(Touch *touch) {
    
    ball->stopMoveX();
}

#pragma mark- Physics Event

/**
 * 볼 & 깃발 충돌
 */
void GameView::onContactFlag(Ball *ball, GameTile *tile) {
    
    auto flag = dynamic_cast<Flag*>(tile);
    
    // 충돌 처리
    switch( flag->getData().tileId ) {
        // Stage Clear
        case TileId::FLAG_CLEAR_PORTAL: {
        } break;
            
        default: break;
    }
}

/**
 * 볼 & 아이템 충돌
 */
void GameView::onContactItem(Ball *ball, GameTile *tile) {
    
    auto item = dynamic_cast<Item*>(tile);
    
    // 아이템 삭제
    removeTile(item);
    
    // 충돌 처리
    switch( item->getData().tileId ) {
        // 소시지
        case TileId::ITEM_SAUSAGE: {
            int star = GAME_MANAGER->getStar() + 1;
            star = MIN(3, star);
            
            GAME_MANAGER->setStar(star);
            stageProgressBar->setStar(star);
            
            auto portal = dynamic_cast<ClearPortal*>(getTiles(TileId::FLAG_CLEAR_PORTAL)[0]);
            portal->setStar(star);
        } break;
            
        // 더블 점프
        case TileId::ITEM_DOUBLE_JUMP: {
            
        } break;
            
        default: break;
    }
}

/**
 * 볼 & 브릭 충돌
 */
void GameView::onContactBlock(Ball *ball, GameTile *tile, Vec2 contactPoint, PhysicsCategory category) {
    
    auto block = dynamic_cast<Block*>(tile);
    
    // 충돌 처리
    switch( block->getData().tileId ) {
        // Normal
        case TileId::BLOCK_NORMAL: {
            if( category == PhysicsCategory::BLOCK_TOP ) {
                // 포털 오픈됨 && 포털 아래칸 충돌 => 스테이지 클리어
                auto portal = dynamic_cast<ClearPortal*>(getTiles(TileId::FLAG_CLEAR_PORTAL)[0]);
                auto portalBelowTile = getTile(portal->getData().p + Vec2(0,-1));
                
                if( portal->isOpened() && portalBelowTile == block ) {
                    ball->setCollisionLocked(true);
                    ball->setSyncLocked(true);
                    
                    auto portalBox = SBNodeUtils::getBoundingBoxInWorld(portal);
                    ball->setPositionY(portalBox.getMinY() + ball->getContentSize().height*0.5f);
                    
                    SBDirector::postDelayed(this, [=]() {
                        auto fadeOut = FadeOut::create(0.3f);
                        auto callFunc = CallFunc::create([=]() {
                            GameManager::onStageClear();
                        });
                        ball->runAction(Sequence::create(fadeOut, callFunc, nullptr));
                    }, 0.2f);
                }
            }
        } break;
            
        // 드랍 블럭
        case TileId::BLOCK_DROP_1:
        case TileId::BLOCK_DROP_2: {
            if( category == PhysicsCategory::BLOCK_TOP ) {
                removeTile(block);
            }
        } break;
        
        // 데스 블럭
        case TileId::BLOCK_DEATH: {
            if( category == PhysicsCategory::BLOCK_TOP ||
                category == PhysicsCategory::BLOCK_SIDE ) {
                ball->setCollisionLocked(true);
                ball->setSyncLocked(true);
                
                if( category == PhysicsCategory::BLOCK_TOP ) {
                    ball->setPositionY(SB_BOUNDING_BOX_IN_WORLD(block).getMaxY() + ball->getContentSize().height*0.5f);
                }
                
                SBDirector::postDelayed(this, [=]() {
                    GameManager::onGameOver(false);
                }, 0.2f);
            }
        } break;
            
        // 점프 블럭
        case TileId::BLOCK_JUMP: {
        } break;
            
        default: break;
    }
}

void GameView::onContactFloor(Ball *ball) {
    
    ball->setCollisionLocked(true);
    ball->setSyncLocked(true);
    
    SBDirector::postDelayed(this, [=]() {
        GameManager::onGameOver(false);
    }, 0.1f);
}

#pragma mark- Initialize

/**
 * 물리 영역 초기화
 */
void GameView::initPhysics() {
    
    setSyncLocked(true);
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->onContactFlag         = CC_CALLBACK_2(GameView::onContactFlag, this);
    listener->onContactItem         = CC_CALLBACK_2(GameView::onContactItem, this);
    listener->onContactBlock        = CC_CALLBACK_4(GameView::onContactBlock, this);
//    listener->onContactWall         = CC_CALLBACK_1(Ball::onContactWall, this);
    listener->onContactFloor        = CC_CALLBACK_1(GameView::onContactFloor, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    world = PHYSICS_MANAGER->initWorld();
    
    // Wall & Floor
    auto STAGE = GAME_MANAGER->getStage();
    
    auto MAP_CONTENT_SIZE = SB_WIN_SIZE;
    MAP_CONTENT_SIZE.width = MAX(STAGE.mapContentSize.width, MAP_CONTENT_SIZE.width);
    
    auto MAP_POSITION = Vec2MC(MAP_CONTENT_SIZE, 0, 0);
    
    b2BodyDef bodyDef;
    bodyDef.position = PTM(MAP_POSITION);
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = world->CreateBody(&bodyDef);
    setBody(body);
    
    float dt     = -STAGE.tileSize.width*1.5f; // 바닥 충돌을 자연스럽게 보이기 위해 확장
    float left   = PTM(-MAP_CONTENT_SIZE.width*0.5f + dt);
    float right  = PTM( MAP_CONTENT_SIZE.width*0.5f - dt);
    float bottom = PTM(-MAP_CONTENT_SIZE.height*0.5f + dt);
    float top    = PTM( MAP_CONTENT_SIZE.height*0.5f - dt);
    
    b2Vec2 vectors[4][2] = {
        { b2Vec2(left, bottom), b2Vec2(left, top) },          // left
        { b2Vec2(right, bottom), b2Vec2(right, top) },        // right
        { b2Vec2(left, bottom), b2Vec2(right, bottom) },      // bottom
        { b2Vec2(left, top), b2Vec2(right, top) },            // top
    };
    
    PhysicsCategory categorys[] = {
        PhysicsCategory::WALL_LEFT,
        PhysicsCategory::WALL_RIGHT,
        PhysicsCategory::FLOOR,
        PhysicsCategory::WALL_TOP,
    };
    
    for( int i = 0; i < 4; ++i ) {
        b2Vec2 v1 = vectors[i][0];
        b2Vec2 v2 = vectors[i][1];
        
        b2EdgeShape shape;
        shape.Set(v1, v2);
        
        b2Filter filter;
        filter.categoryBits = categorys[i];
        filter.maskBits = PHYSICS_MASK_BITS_WALL;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 0.1f;      // 밀도
        fixtureDef.restitution = 0;     // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
        fixtureDef.friction = 0;        // 마찰력
        fixtureDef.filter = filter;
        body->CreateFixture(&fixtureDef);
    }
    
#if DEBUG_DRAW_PHYSICS
    // DebugDrawView
    auto view = DebugDrawView::create(world);
    view->setCameraMask((unsigned short)CAMERA_FLAG_MAP);
    view->setVisible(false);
    view->setTag(Tag::DEBUG_DRAW_VIEW);
    addChild(view, SBZOrder::MIDDLE);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//        flags += b2Draw::e_jointBit;
//        flags += b2Draw::e_aabbBit;
//        flags += b2Draw::e_pairBit;
//        flags += b2Draw::e_centerOfMassBit;
    view->getDebugDraw()->SetFlags(flags);
    
    // display label
    infoLabel = Label::createWithTTF("", FONT_ROBOTO_BLACK, 60, Size::ZERO,
                                     TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
    infoLabel->setVisible(false);
    infoLabel->setTextColor(Color4B::WHITE);
    infoLabel->setAnchorPoint(ANCHOR_BR);
    infoLabel->setPosition(Vec2BR(-50, -20));
    addChild(infoLabel, SBZOrder::MIDDLE);
    
    maxVelocity = Vec2(INT_MIN, INT_MIN);
    minVelocity = Vec2(INT_MAX, INT_MAX);
    
    // toggle button
    auto toggleBtn = GameUIHelper::createFontButton("DebugDraw", Size(300,200));
    toggleBtn->setAnchorPoint(ANCHOR_TL);
    toggleBtn->setPosition(Vec2TL(10, 10));
    addChild(toggleBtn, SBZOrder::MIDDLE);
    
    toggleBtn->setOnClickListener([=](Node*) {
        infoLabel->setVisible(!infoLabel->isVisible());
        view->setVisible(!view->isVisible());
    });
#endif
}

/**
 * 카메라 초기화
 */
void GameView::initCamera() {
    
    // setCameraMask((unsigned short)CameraFlag::USER1, false);
    
    auto bgCamera = Camera::create();
    bgCamera->setCameraFlag(CAMERA_FLAG_BG);
    bgCamera->setDepth(CAMERA_DEPTH_BG);
    addChild(bgCamera);
    
    auto mapCamera = Camera::create();
    mapCamera->setCameraFlag(CAMERA_FLAG_MAP);
    mapCamera->setDepth(CAMERA_DEPTH_MAP);
    addChild(mapCamera);
    
    auto camControlNode = Node::create();
    camControlNode->setPositionNormalized(Vec2(.5,.5));
    addChild(camControlNode);
    
    CCLOG("first cam controll: %f", camControlNode->getPositionX());
    
    auto camNode = Node::create();
    camNode->setPositionZ(mapCamera->getPosition3D().z);
    camControlNode->addChild(camNode);
    
    mapCameraInfo.camera = mapCamera;
    mapCameraInfo.camControlNode = camControlNode;
    mapCameraInfo.camNode = camNode;
}

/**
 * 배경 초기화
 */
void GameView::initBg() {
    
    auto stage = GAME_MANAGER->getStage();
//    auto bgWidth = MAX(stage.mapContentSize.width, SB_WIN_SIZE.width);
//    auto bgHeight = MAX(stage.mapContentSize.height, SB_WIN_SIZE.height);
    auto bgWidth = SB_WIN_SIZE.width;
    auto bgHeight = SB_WIN_SIZE.height;
    
    auto bg = Sprite::create(ResourceHelper::getWorldBackgroundImage(stage.world));
    bg->setCameraMask((unsigned short)CAMERA_FLAG_BG);
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0,0));
    bg->setScaleX(bgWidth / bg->getContentSize().width);
    bg->setScaleY(bgHeight / bg->getContentSize().height);
    addChild(bg, -1);
    
    // 스테이지 진행도
    stageProgressBar = StageProgressBar::create();
    addChild(stageProgressBar, ZOrder::MENU);
}

/**
 * 타일 초기화
 */
void GameView::initTiles() {
    
    auto stage = GAME_MANAGER->getStage();
    
    for( auto tileData : stage.tiles ) {
        if( tileData.tileId == TileId::NONE ) {
            continue;
        }
        
        GameTile *tile = nullptr;
        
        switch( tileData.tileId ) {
            case TileId::FLAG_START: {
                tile = Flag::create(tileData);
            } break;
            case TileId::FLAG_CLEAR_PORTAL: {
                tile = ClearPortal::create(tileData);
            } break;
                
            case TileId::ITEM_SAUSAGE:
            case TileId::ITEM_DOUBLE_JUMP: {
                tile = Item::create(tileData);
            } break;
                
            case TileId::BLOCK_NORMAL:
            case TileId::BLOCK_DROP_1:
            case TileId::BLOCK_DROP_2:
            case TileId::BLOCK_DEATH:
            case TileId::BLOCK_JUMP: {
                tile = Block::create(tileData);
            } break;
                
            default: break;
        }
        
        if( tile ) {
            addTile(tile);
        }
    }
}

/**
 * 볼(캐릭터) 초기화
 */
void GameView::initBall() {
 
    auto stage = GAME_MANAGER->getStage();
    auto flag = stage.getTiles(TileId::FLAG_START)[0];
    
    ball = Ball::create(stage);
    ball->setCameraMask((unsigned short)CAMERA_FLAG_MAP);
    addChild(ball, ZOrder::BALL);
    
    auto pos = convertTilePosition(stage, flag.x, flag.y+1);
//    pos.y -= (stage.tileSize.height - ball->getContentSize().height) / 2;
//    pos.y += 1;
    ball->setFirstPosition(pos);
}

/**
 * 게임 이벤트 리스너 초기화
 */
void GameView::initGameListener() {
    
    GameEventList events({
        GameEvent::RESET,
        GameEvent::EXIT,
        GameEvent::PAUSE,
        GameEvent::RESUME,
        GameEvent::STAGE_CHANGED,
        GameEvent::STAGE_RESTART,
        GameEvent::STAGE_CLEAR,
        GameEvent::MOVE_NEXT_STAGE,
        GameEvent::MOVE_NEXT_STAGE_FINISHED,
    });
    
    GameManager::addEventListener(events, [=](GameEvent event, void *userData) {
        
        switch( event ) {
            case GameEvent::RESET:     this->onGameReset();         break;
            case GameEvent::EXIT:      this->onGameExit();         break;
            case GameEvent::PAUSE:     this->onGamePause();         break;
            case GameEvent::RESUME:    this->onGameResume();        break;
                
            case GameEvent::STAGE_CHANGED: {
                auto stage = (StageData*)userData;
                this->onStageChanged(*stage);
            } break;
                
            case GameEvent::STAGE_RESTART: {
                auto stage = (StageData*)userData;
                this->onStageRestart(*stage);
            } break;
                
            case GameEvent::STAGE_CLEAR: {
                auto stage = (StageData*)userData;
                this->onStageClear(*stage);
            } break;
                
            case GameEvent::MOVE_NEXT_STAGE:            this->onMoveNextStage();          break;
            case GameEvent::MOVE_NEXT_STAGE_FINISHED:   this->onMoveNextStageFinished();  break;
                
            default: break;
        }
    }, this);
}

void GameView::initTouchListener() {
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [=](Touch *touch, Event *unusedEvent) -> bool {
        
        if( !isTouchEnabled ) {
            return false;
        }
        
        this->onTouchBegan(touch);
        return true;
    };
    touchListener->onTouchMoved = [=](Touch *touch, Event *unusedEvent) {
    };
    touchListener->onTouchEnded = [=](Touch *touch, Event *unusedEvent) {
        this->onTouchEnded(touch);
    };
    touchListener->onTouchCancelled = [=](Touch *touch, Event *unusedEvent) {
        this->onTouchEnded(touch);
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}
