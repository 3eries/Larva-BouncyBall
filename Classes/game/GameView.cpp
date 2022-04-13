//
//  GameView.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameView.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "GameUIHelper.hpp"
#include "StageManager.hpp"
#include "SceneManager.h"

#include "object/Ball.hpp"
#include "object/tile/Flag.hpp"
#include "object/tile/ClearPortal.hpp"
#include "object/tile/Item.hpp"
#include "object/tile/Block.hpp"
#include "object/tile/BlockDrop.hpp"
#include "object/tile/BlockMove.hpp"
#include "object/tile/BlockMoveRail.hpp"

#include "object/StageProgressBar.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define SCHEDULER_UPDATE_CAMERA                     "SCHEDULER_UPDATE_CAMERA"

#define TAP_INTERVAL                                (0.35f / 2) // 탭 판단 시간
#define TAP_DIST                                    300         // 탭 판단 거리

#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG == 1
#define DEBUG_DRAW_PHYSICS                  1
#else
#define DEBUG_DRAW_PHYSICS                  0
#endif // COCOS2D_DEBUG == 1

GameView::GameView(): SBPhysicsObject(this),
isTouchEnabled(false),
tapCount(0) {
}

GameView::~GameView() {
    
    touches.clear();
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
    
    return true;
}

void GameView::onEnter() {
    
    Log::i("GameView::onEnter");
    
    Node::onEnter();
}

void GameView::onEnterTransitionDidFinish() {
    
    Node::onEnterTransitionDidFinish();
    
    initTouchListener();
    
    // 튜토리얼
    auto stage = GAME_MANAGER->getStage();
    
    switch( stage.stage ) {
        case TUTORIAL_STAGE_TOUCH: {
            auto anim = SBSkeletonAnimation::create(DIR_IMG_GAME + "tutorial.json");
            anim->setTag(Tag::TUTORIAL);
            anim->setPosition(Vec2MC(0,0));
            addChild(anim, SBZOrder::BOTTOM);

            anim->setAnimation(0, "touch_left", true);
        } break;
            
        case TUTORIAL_STAGE_JUMP: {
            auto startFlag = stage.getTiles(TileId::FLAG_START)[0];
            
            auto img = Sprite::create(DIR_IMG_GAME + "tutorial_jump.png");
            img->setTag(Tag::TUTORIAL);
            img->setAnchorPoint(ANCHOR_BL);
            img->setPosition(convertTilePosition(stage, startFlag.x+1, startFlag.y+2) +
                             Vec2(-14, -30));
            addChild(img, SBZOrder::BOTTOM);
            
            auto move = Sequence::create(MoveTo::create(1.0f, img->getPosition() + Vec2(0, 10)),
                                         MoveTo::create(1.0f, img->getPosition()),
                                         nullptr);
            img->runAction(RepeatForever::create(move));
        } break;
            
        case TUTORIAL_STAGE_DEATH_BLOCK: {
            auto startFlag = stage.getTiles(TileId::FLAG_START)[0];
            
            auto img = Sprite::create(DIR_IMG_GAME + "tutorial_warning.png");
            img->setTag(Tag::TUTORIAL);
            img->setAnchorPoint(ANCHOR_MT);
            img->setPosition(convertTilePosition(stage, startFlag.x+3, startFlag.y-1) +
                             Vec2(-8, -64));
            addChild(img, SBZOrder::BOTTOM);
            
            auto move = Sequence::create(MoveTo::create(1.0f, img->getPosition() + Vec2(0, -10)),
                                         MoveTo::create(1.0f, img->getPosition()),
                                         nullptr);
            img->runAction(RepeatForever::create(move));
        } break;
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
    
    isTouchEnabled = true;
}

/**
 * 게임 종료
 */
void GameView::onGameExit() {
    
#if DEBUG_DRAW_PHYSICS
    removeChildByTag(Tag::DEBUG_DRAW_VIEW);
#endif
    
    SBNodeUtils::recursivePauseSchedulerAndActions(this);
    removeListeners(this);
}

/**
 * 게임 일시정지
 */
void GameView::onGamePause() {
    
    ball->pause();
    
    for( auto tile : tiles ) {
        SBNodeUtils::recursivePause(tile);
    }
    
    getScheduler()->pauseTarget(this);
    getActionManager()->pauseTarget(this);
}

/**
 * 게임 재개
 */
void GameView::onGameResume() {
    
    getScheduler()->resumeTarget(this);
    getActionManager()->resumeTarget(this);
    
    ball->resume();
    
    for( auto tile : tiles ) {
        SBNodeUtils::recursiveResume(tile);
    }
}

/**
 * 게임 오버 전
 */
void GameView::onPreGameOver() {
    
    ball->setCollisionLocked(true);
    ball->setSyncLocked(true);
    
    isTouchEnabled = false;
    touches.clear();
    
    // removeListeners(this);
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
        case TileType::BLOCK:   z = ZOrder::TILE_BLOCK;     break;
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
    
    touches.pushBack(touch);
    
    auto direction = getBallDirection(touch);
    CCLOG("터치 시작: %s", toString(direction).c_str());
    
    ball->prepareTouch();
    ball->setDirection(direction);
    
    // 탭 체크
    if( touch->getID() == 0 ) {
        // 첫번째 탭 터치 시작
        if( tapCount == 0 ) {
            firstTap.setTouchBegan(touch->getLocation());
        }
        // 두번째 탭 터치 시작
        else if( tapCount == 1 ) {
            secondTap.setTouchBegan(touch->getLocation());
            
            // 첫번째 탭과 이어지는지 체크
            if( !isTap(firstTap.p2, secondTap.p1, firstTap.t2, secondTap.t1) ) {
                CCLOG("첫번째 탭 재시작!");
                // 첫번째 탭으로 변경
                tapCount = 0;
                firstTap = secondTap;
            }
        }
        else {
            CCASSERT(false, "GameView::onTouchBegan tap count error.");
        }
    }
}

void GameView::onTouchEnded(Touch *touch) {
    
    CCLOG("터치 종료: %d", (int)touches.size());
    
    touches.eraseObject(touch, true);
    
    // 탭 체크
    if( touch->getID() == 0 ) {
        // 첫번째 탭 터치 종료
        if( tapCount == 0 ) {
            firstTap.setTouchEnded(touch->getLocation());
            
            if( isTap(firstTap.p1, firstTap.p2, firstTap.t1, firstTap.t2) ) {
                CCLOG("첫번째 탭!");
                tapCount++;
            }
        }
        // 두번째 탭 터치 종료
        else if( tapCount == 1 ) {
            secondTap.setTouchEnded(touch->getLocation());
            
            if( isTap(secondTap.p1, secondTap.p2, secondTap.t1, secondTap.t2) ) {
                CCLOG("두번째 탭!");
                tapCount++;
            } else {
                CCLOG("두번째 탭 취소!");
                tapCount = 0;
            }
        }
        else {
            CCASSERT(false, "GameView::onTouchEnded tap count error.");
        }
        
        if( tapCount == 2 ) {
            tapCount = 0;
            
            // 더블 점프
            if( ball->hasState(Ball::State::DOUBLE_JUMP_READY) ) {
                // 튜토리얼 제거
                auto tutorial = getChildByTag<SBSkeletonAnimation*>(Tag::TUTORIAL);
                
                if( GAME_MANAGER->getStage().stage == TUTORIAL_STAGE_DOUBLE_JUMP && tutorial ) {
                    GAME_MANAGER->removeState(GameState::TUTORIAL_PAUSE);
                    
                    ball->resume();
                    PHYSICS_MANAGER->resumeScheduler();
                    
                    tutorial->removeFromParent();
                }
                
                ball->doubleJumpStart();
            }
        }
    }
    
    if( touches.size() == 0 ) {
        ball->stopHorizontal(0.05f);
    } else {
        CCLOG("잔여 터치");
        auto remainTouch = touches.at(touches.size()-1);
        ball->setDirection(getBallDirection(remainTouch));
    }
}

bool GameView::isTap(const Vec2 &p1, const Vec2 &p2, double t1, double t2) {
    return t2 - t1 <= TAP_INTERVAL && p2.getDistance(p1) <= TAP_DIST;
}

BallDirection GameView::getBallDirection(cocos2d::Touch *touch) {
    bool isLeft = (touch->getLocation().x < SB_WIN_SIZE.width*0.5f);
    return isLeft ? BallDirection::LEFT : BallDirection::RIGHT;
}

#pragma mark- Physics Event

/**
 * 볼 & 깃발 충돌
 */
void GameView::onContactFlag(Ball *ball, GameTile *tile) {
    
    auto flag = dynamic_cast<Flag*>(tile);
    
    switch( flag->getData().tileId ) {
        // 클리어 포털
        case TileId::FLAG_CLEAR_PORTAL: {
            auto portal = dynamic_cast<ClearPortal*>(flag);
            
            // 웨이브 상태에서 클리어 포털과 충돌하는지 체크
            if( portal->isOpened() && ball->hasState(Ball::State::WAVE) ) {
                auto portalBox = SB_BOUNDING_BOX_IN_WORLD(portal);
                auto characterBox = SB_BOUNDING_BOX_IN_WORLD(ball);
                
                // 클리어 포털 중점이 캐릭터 박스에 포함되면 클리어
                if( characterBox.containsPoint(Vec2(portalBox.getMidX(), portalBox.getMidY())) ) {
                    onContactClearPortal(portal, false);
                }
            }
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
    
    switch( item->getData().tileId ) {
        // 소시지
        case TileId::ITEM_SAUSAGE: {
            int star = GAME_MANAGER->getStar() + 1;
            star = MIN(3, star);
            
            GAME_MANAGER->setStar(star);
            stageProgressBar->setStar(star);
            
            auto portal = dynamic_cast<ClearPortal*>(getTiles(TileId::FLAG_CLEAR_PORTAL)[0]);
            portal->setStar(star);
            
            // 효과음
            SBAudioEngine::playEffect(SOUND_SAUSAGE);
            
            // 튜토리얼 업데이트
            switch( GAME_MANAGER->getStage().stage ) {
                // 터치 튜토리얼
                case TUTORIAL_STAGE_TOUCH: {
                    // 첫번째 소시지 획득 시 오른쪽 터치로 변경
                    if( star == 1 ) {
                        removeChildByTag(Tag::TUTORIAL);
                        
                        auto tutorial = SBSkeletonAnimation::create(DIR_IMG_GAME + "tutorial.json");
                        tutorial->setTag(Tag::TUTORIAL);
                        tutorial->setPosition(Vec2MC(0,0));
                        addChild(tutorial, SBZOrder::BOTTOM);
                        
                        tutorial->setAnimation(0, "touch_right", true);
                    }
                } break;
                    
                // 점프, 데스블럭 튜토리얼
                case TUTORIAL_STAGE_JUMP:
                case TUTORIAL_STAGE_DEATH_BLOCK: {
                    // 두번째 소시지 획득 시 튜토리얼 제거
                    if( star == 2 ) {
                        auto tutorial = getChildByTag(Tag::TUTORIAL);
                        tutorial->stopAllActions();
                        
                        tutorial->runAction(Sequence::create(FadeOut::create(0.4f),
                                                             RemoveSelf::create(), nullptr));
                    }
                } break;
            }
        } break;
            
        // 더블 점프
        case TileId::ITEM_DOUBLE_JUMP: {
            // 효과음
            SBAudioEngine::playEffect(SOUND_DOUBLE_JUMP_GET);
            
            // 튜토리얼, 물리 세계 일시정지
            if( GAME_MANAGER->getStage().stage == TUTORIAL_STAGE_DOUBLE_JUMP ) {
                SBDirector::postDelayed(this, [=]() {
                    GAME_MANAGER->addState(GameState::TUTORIAL_PAUSE);
                    
                    PHYSICS_MANAGER->pauseScheduler();
                    ball->pause();
                }, 0.1f, true);
                
                auto tutorial = SBSkeletonAnimation::create(DIR_IMG_GAME + "tutorial.json");
                tutorial->setTag(Tag::TUTORIAL);
                tutorial->setPosition(Vec2MC(0,0));
                addChild(tutorial, SBZOrder::BOTTOM);
                
                tutorial->setAnimation(0, "touch_double", true);
            }
        } break;
            
        default: break;
    }
}

/**
 * 볼 & 블럭 충돌
 */
void GameView::onContactBlock(Ball *ball, GameTile *tile, Vec2 contactPoint, PhysicsCategory category) {
    
    auto block = dynamic_cast<Block*>(tile);
    
    switch( block->getData().blockType ) {
        // Normal
        case BlockType::NORMAL: {
            if( category == PhysicsCategory::BLOCK_TOP ) {
                // 포털 오픈됨 && 포털 아래칸 충돌 => 스테이지 클리어
                auto portal = dynamic_cast<ClearPortal*>(getTiles(TileId::FLAG_CLEAR_PORTAL)[0]);
                auto portalBelowTile = getTile(portal->getData().p + Vec2(0,-1));
                
                if( portal->isOpened() && portalBelowTile == block ) {
                    onContactClearPortal(portal, true);
                }
            }
        } break;
            
        // 드랍 블럭
        case BlockType::DROP: {
            if( category == PhysicsCategory::BLOCK_TOP ) {
                auto dropBlock = dynamic_cast<BlockDrop*>(block);
                dropBlock->dropEffect();
                
                removeTile(dropBlock);
            }
        } break;
        
        // 데스 블럭
        case BlockType::DEATH: {
            vector<PhysicsCategory> checkCategorys({
                PhysicsCategory::BLOCK_TOP,
                PhysicsCategory::BLOCK_SIDE,
            });
            
            if( block->getData().tileId == TileId::BLOCK_DEATH_4D ) {
                checkCategorys.push_back(PhysicsCategory::BLOCK_BOTTOM);
            }
            
            for( auto checkCategory : checkCategorys ) {
                if( category == checkCategory ) {
                    SBAudioEngine::playEffect(SOUND_GAME_OVER_DEATH_BLOCK);
                    
                    if( category == PhysicsCategory::BLOCK_TOP ) {
                        ball->setPositionY(SB_BOUNDING_BOX_IN_WORLD(block).getMaxY() + ball->getContentSize().height*0.5f);
                    }
                    
                    onPreGameOver();
                    
                    // 스턴 연출
                    SBDirector::getInstance()->setScreenTouchLocked(true);
  
                    ball->effectStun([=]() {
                        SBDirector::postDelayed(this, [=]() {
                            SBDirector::getInstance()->setScreenTouchLocked(false);
                            GameManager::onGameOver(GameOverType::DEATH_BLOCK);
                        }, 0.1f, false);
                    });
                    
                    break;
                }
            }
        } break;
            
        default: break;
    }
}

/**
 * 볼 & 바닥 충돌
 */
void GameView::onContactFloor(Ball *ball) {
    
    SBAudioEngine::playEffect(SOUND_GAME_OVER_FALL);
    
    onPreGameOver();
    
    SBDirector::postDelayed(this, [=]() {
        GameManager::onGameOver(GameOverType::FALL);
    }, 0.1f, true);
}

/**
 * 볼 & 클리어 포털 충돌
 */
void GameView::onContactClearPortal(GameTile *tile, bool isContactPortalBelowTile) {
    
    auto portal = dynamic_cast<ClearPortal*>(tile);
    
    if( !portal->isOpened() ) {
        return;
    }
    
    if( ball->isCollisionLocked() ) {
        // 클리어 중복 방지
        return;
    }
    
    ball->setCollisionLocked(true);
    ball->setSyncLocked(true);
    
    isTouchEnabled = false;
    
    if( isContactPortalBelowTile ) {
        auto portalBox = SBNodeUtils::getBoundingBoxInWorld(portal);
        ball->setPositionY(portalBox.getMinY() + ball->getContentSize().height*0.5f);
    }
    
    SBDirector::postDelayed(this, [=]() {
        SBAudioEngine::playEffect(SOUND_CLEAR_PORTAL);
        
        auto fadeOut = FadeOut::create(0.3f);
        auto callFunc = CallFunc::create([=]() {
            GameManager::onStageClear();
        });
        ball->runAction(Sequence::create(fadeOut, callFunc, nullptr));
    }, 0.2f, true);
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
    auto toggleBtn = GameUIHelper::createFontButton("DebugDraw", Size(300,180));
    toggleBtn->setAnchorPoint(ANCHOR_TL);
    toggleBtn->setPosition(Vec2TL(10, -40));
    toggleBtn->setOpacity(255*0.3f);
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
        
        // Block
        if( tileData.isBlock() ) {
            switch( tileData.blockType ) {
                case BlockType::NORMAL:
                case BlockType::DEATH:
                case BlockType::JUMP:
                case BlockType::WAVE: {
                    tile = Block::create(tileData);
                } break;
                    
                case BlockType::DROP: {
                    tile = BlockDrop::create(tileData);
                } break;
                    
                case BlockType::MOVE: {
                    tile = BlockMoveRail::create(tileData);
                    
                    // START 블럭일 때 무브 블럭도 생성
                    if( tileData.tileId == TileId::BLOCK_MOVE_START ) {
                        auto moveBlock = BlockMove::create(tileData, stage.getPairMoveEndBlock(tileData));
                        addTile(moveBlock);
                        
                        // ZOrder 변경
                        moveBlock->setLocalZOrder(ZOrder::TILE_BLOCK + 1);
                    }
                } break;
                    
                default: break;
            }
        }
        // Flag & Item
        else {
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
                    
                default: break;
            }
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
    });
    
    GameManager::addEventListener(events, [=](GameEvent event, void *userData) {
        
        switch( event ) {
            case GameEvent::RESET:     this->onGameReset();         break;
            case GameEvent::EXIT:      this->onGameExit();          break;
            case GameEvent::PAUSE:     this->onGamePause();         break;
            case GameEvent::RESUME:    this->onGameResume();        break;
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

        if( touch->getID() >= 2 ) {
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
