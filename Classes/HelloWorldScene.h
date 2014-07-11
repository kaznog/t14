#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "Constants.h"
#include "GroundLayer.h"
#include "InterfaceLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HelloWorld : public cocos2d::Layer
{
public:
    ~HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    Size _winSize;
};

#endif // __HELLOWORLD_SCENE_H__
