//
//  Car.h
//  b2dcar
//
//  Created by 野口一也 on 2014/07/09.
//
//

#ifndef __b2dcar__Car__
#define __b2dcar__Car__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Constants.h"
#include "Tire.h"

USING_NS_CC;

class Car {
    b2RevoluteJoint *flJoint, *frJoint;
public:
    std::vector<Tire*> m_tires;
    b2Body* m_body;
    Car(b2World* world, Vec2 position);
    ~Car();
    
    void update(int controlState);
};
#endif /* defined(__b2dcar__Car__) */
