//
//  CameraTest.h
//  b2dcar
//
//  Created by 野口一也 on 2014/07/11.
//
//

#ifndef __b2dcar__CameraTest__
#define __b2dcar__CameraTest__

#include "cocos2d.h"
#include "GLES-Render.h"
#include "Constants.h"
#include "Car.h"
#include "InterfaceLayer.h"

USING_NS_CC;

class CameraTest : public cocos2d::Layer, public InterfaceLayerDelegate
{
public:
    CameraTest();
    ~CameraTest();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(CameraTest);
    
    virtual void update(float dt);
    
    // Layer::draw メソッドをオーバーライド
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    virtual void TouchBegan(int CtrlState);
    virtual void TouchEnded(int CtrlState);
    
private:
    void setViewPointCenter();
    void onDraw(const Mat4 &transform, uint32_t flags);
    Size _winSize;
    b2World* _world;
    Car* _car;
    void initPhysics();
    GLESDebugDraw* _debugDraw;
    CustomCommand _customCmd;
    
    int _controlState;
    ActionCamera* _camera;
};

#endif /* defined(__b2dcar__CameraTest__) */
