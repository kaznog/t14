#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer, 0);
    
    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto director = Director::getInstance();
    _winSize = director->getWinSize();
 
    _ground = GroundLayer::create();
    this->addChild(_ground, 0);
    
    auto interface = InterfaceLayer::create();
    interface->setDelegate(_ground);
    
    this->addChild(interface, 1);

    return true;
}
