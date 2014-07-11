//
//  InterfaceLauer.h
//  b2dcar
//
//  Created by 野口一也 on 2014/07/10.
//
//

#ifndef __b2dcar__InterfaceLauer__
#define __b2dcar__InterfaceLauer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum {
    CTRL_LEFT = 0x1,
    CTRL_RIGHT = 0x2,
    CTRL_ACCEL = 0x4,
    CTRL_BACK  = 0x8
};

class InterfaceLayerDelegate
{
public:
    virtual void TouchBegan(int CtrlState) = 0;
    virtual void TouchEnded(int CtrlState) = 0;
};

class InterfaceLayer : public cocos2d::Layer
{
public:
    InterfaceLayer();
    ~InterfaceLayer();
    virtual bool init();
    
    CREATE_FUNC(InterfaceLayer);
    void setDelegate(InterfaceLayerDelegate* pDelegate);

    void menuCloseCallback(cocos2d::Ref* pSender);
    void addTargetWithActionForControlEvents(ControlButton* button, Control::Handler downAction, Control::Handler upAction);
    void leftTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void rightTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void accelTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void backTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void leftTouchUpAction(Ref *senderz, Control::EventType controlEvent);
    void rightTouchUpAction(Ref *senderz, Control::EventType controlEvent);
    void accelTouchUpAction(Ref *senderz, Control::EventType controlEvent);
    void backTouchUpAction(Ref *senderz, Control::EventType controlEvent);
private:
    InterfaceLayerDelegate* _delegate;
};

#endif /* defined(__b2dcar__InterfaceLauer__) */
