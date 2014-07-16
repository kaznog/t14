//
//  CameraTest.cpp
//  b2dcar
//
//  Created by 野口一也 on 2014/07/11.
//
//

#include "CameraTest.h"

Scene* CameraTest::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CameraTest::create();
    
    // add layer as a child to scene
    scene->addChild(layer, 0);
    
    auto interface = InterfaceLayer::create();
    interface->setDelegate(layer);
    
    scene->addChild(interface, 1);
    
    // return the scene
    return scene;
}

CameraTest::CameraTest()
: _camera(nullptr)
, _car(nullptr)
{
    
}

CameraTest::~CameraTest()
{
    CC_SAFE_DELETE(_car);
    CC_SAFE_RELEASE(_camera);
}

bool CameraTest::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto director = Director::getInstance();
    Size _winSize = director->getWinSize();
    
    auto bg = Sprite::create("HelloWorld.png");
    bg->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    this->addChild(bg, -1);
    
    _camera = new ActionCamera();
    _camera->startWithTarget(this);
    
    initPhysics();
    scheduleUpdate();
    return true;
}

void CameraTest::initPhysics()
{
    // 重力を作成
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    // Worldを作成
    _world = new b2World(gravity);
    _world->SetGravity(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    _debugDraw = new GLESDebugDraw(PTM_RATIO);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_pairBit;
    //    flags += b2Draw::e_aabbBit;
    //    flags += b2Draw::e_centerOfMassBit;
    _debugDraw->SetFlags(flags);
    _world->SetDebugDraw(_debugDraw);
    
    _car = new Car(_world, Vec2((_winSize.width/2) / PTM_RATIO, (_winSize.height/2) / PTM_RATIO));
    
}

void CameraTest::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Layer::draw(renderer, transform, flags);
    
    _customCmd.init(_globalZOrder);
    _customCmd.func = CC_CALLBACK_0(CameraTest::onDraw, this, transform, flags);
    renderer->addCommand(&_customCmd);
}

void CameraTest::onDraw(const Mat4 &transform, uint32_t flags)
{
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    _world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void CameraTest::update(float dt)
{
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 10;
    int32 positionIterations = 10;
    _world->Step(timeStep, velocityIterations, positionIterations);
    _world->ClearForces();
    setViewPointCenter();
    
    _car->update(_controlState);
}

void CameraTest::setViewPointCenter()
{
//    Vec3 center = _camera->getCenter();
//    b2Vec2 carPos = _car->m_body->GetPosition();
//    Vec2 CenterPos = Vec2(carPos.x * PTM_RATIO - (_winSize.width / 2), carPos.y * PTM_RATIO - (_winSize.height / 2));
//    _camera->setCenter(Vec3(CenterPos.x, CenterPos.y, 0));
    //    _camera = new ActionCamera();
    //    _camera->startWithTarget(parent);
    float rad = _car->m_body->GetAngle() + CC_DEGREES_TO_RADIANS(90);
//    float rad = -1 * CC_DEGREES_TO_RADIANS(angle);
    float x = cos(rad);
    float y = sin(rad);
    _camera->setUp(Vec3(x, y, 0));
    //    _camera->release();
}

void CameraTest::TouchBegan(int CtrlState)
{
    switch (CtrlState) {
        case CTRL_LEFT:
            _controlState &= ~CTRL_RIGHT;
            _controlState |= CTRL_LEFT;
            break;
            
        case CTRL_RIGHT:
            _controlState &= ~CTRL_LEFT;
            _controlState |= CTRL_RIGHT;
            break;
            
        case CTRL_ACCEL:
            _controlState |= CTRL_ACCEL;
            break;
            
        default:
            break;
    }
}

void CameraTest::TouchEnded(int CtrlState)
{
    switch (CtrlState) {
        case CTRL_LEFT:
            _controlState &= ~CTRL_LEFT;
            break;
            
        case CTRL_RIGHT:
            _controlState &= ~CTRL_RIGHT;
            break;
            
        case CTRL_ACCEL:
            _controlState &= ~CTRL_ACCEL;
            break;
            
        default:
            break;
    }
}
