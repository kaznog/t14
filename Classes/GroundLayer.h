//
//  GroundLayer.h
//  b2dcar
//
//  Created by 野口一也 on 2014/07/11.
//
//

#ifndef __b2dcar__GroundLayer__
#define __b2dcar__GroundLayer__

#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "Constants.h"
#include "Car.h"
#include "InterfaceLayer.h"
#include "rubestuff/b2dJson.h"

class GroundLayer : public cocos2d::Layer, public InterfaceLayerDelegate
{
public:
    GroundLayer();
    ~GroundLayer();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // Layer::draw メソッドをオーバーライド
    virtual void draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(GroundLayer);
    
    virtual void update(float dt);
    virtual void TouchBegan(int CtrlState);
    virtual void TouchEnded(int CtrlState);
    
private:
    void loadWorld();
    void afterLoadProcessing(b2dJson* json);
    void setViewPointCenter();
    Size _winSize;
    b2World* _world;
    Car* _car;
    void initPhysics();
    GLESDebugDraw* _debugDraw;
    CustomCommand _customCmd;
    
    float _scale;
    int _controlState;
};

#endif /* defined(__b2dcar__GroundLayer__) */
