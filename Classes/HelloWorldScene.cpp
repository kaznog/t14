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
 
//    auto scrollview = ScrollView::create(_winSize);
//    scrollview->setDirection(ScrollView::Direction::BOTH);
//    scrollview->setBounceable(false);
//    this->addChild(scrollview, 0);
    
    auto ground = GroundLayer::create();
//    scrollview->setContainer(ground);
//    scrollview->setContentSize(Size(_winSize.width * 2, _winSize.height * 2));
//    scrollview->setContentOffset(-1 * Vec2(_winSize.width/2, _winSize.height/2));
    this->addChild(ground, 0);
    
    auto interface = InterfaceLayer::create();
    interface->setDelegate(ground);
    
    this->addChild(interface, 1);

    return true;
}