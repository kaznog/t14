//
//  Tire.h
//  b2dcar
//
//  Created by 野口一也 on 2014/07/09.
//
//

#ifndef __b2dcar__Tire__
#define __b2dcar__Tire__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Constants.h"

USING_NS_CC;

class Tire {
public:
    b2Body* m_body;
    float m_maxForwardSpeed;
    float m_maxBackwardSpeed;
    float m_maxDriveForce;
    float m_maxLateralImpulse;
    float m_currentTraction;
    
    Tire(b2World* world, b2Vec2 position);
    ~Tire();
    void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse);
    
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void updateFriction();
    void updateDrive(int controlState);
};

#endif /* defined(__b2dcar__Tire__) */
