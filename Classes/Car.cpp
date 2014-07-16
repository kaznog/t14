//
//  Car.cpp
//  b2dcar
//
//  Created by 野口一也 on 2014/07/09.
//
//

#include "Car.h"
#include "InterfaceLayer.h"

Car::Car(b2World* world, Vec2 position)
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(position.x, position.y);
    m_body = world->CreateBody(&bodydef);
    m_body->ResetMassData();
    m_body->SetAngularVelocity(0);
    m_body->SetLinearVelocity(b2Vec2_zero);
    m_body->SetLinearDamping(0);
    m_body->SetAngularDamping(0);
    m_body->SetUserData(this);
    
    b2Vec2 vertices[8];
    vertices[0].Set(  1.5 * CAR_SCALE,  0.0);
    vertices[1].Set(  3.0 * CAR_SCALE,  2.5 * CAR_SCALE);
    vertices[2].Set(  2.8 * CAR_SCALE,  5.5 * CAR_SCALE);
    vertices[3].Set(  1.0 * CAR_SCALE, 10.0 * CAR_SCALE);
    vertices[4].Set( -1.0 * CAR_SCALE, 10.0 * CAR_SCALE);
    vertices[5].Set( -2.8 * CAR_SCALE,  5.5 * CAR_SCALE);
    vertices[6].Set( -3.0 * CAR_SCALE,  2.5 * CAR_SCALE);
    vertices[7].Set( -1.5 * CAR_SCALE,  0.0);
    b2PolygonShape polygonShape;
    polygonShape.Set( vertices, 8 );
    b2Fixture* fixture = m_body->CreateFixture( &polygonShape, 0.1f );
    
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableMotor = false;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero(); // center of tire
    
    float maxForwardSpeed = 250.0 * CAR_SCALE;
    float maxBackwardSpeed = -40 * CAR_SCALE;
    float backTireMaxDriveForce = 300 * CAR_SCALE;
    float frontTireMaxDriveForce = 500 * CAR_SCALE;
    float backTireMaxLateralImpulse = 8.5 * CAR_SCALE;
    float frontTireMaxLateralImpulse= 7.5 * CAR_SCALE;
    
    // back left tire
    b2Vec2 backleftpos = b2Vec2(position.x + (-3.0f * CAR_SCALE), position.y + (0.75f * CAR_SCALE));
    Tire* tire = new Tire(world, backleftpos);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.Initialize(m_body, tire->m_body, backleftpos);
    world->CreateJoint( &jointDef );
    m_tires.push_back(tire);
    
    // back right tire
    b2Vec2 backrightpos = b2Vec2(position.x + (3.0f * CAR_SCALE), position.y + (0.75f * CAR_SCALE));
    tire = new Tire(world, backrightpos);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.Initialize(m_body, tire->m_body, backrightpos);
    world->CreateJoint( &jointDef );
    m_tires.push_back(tire);
    
    // front left tire
    b2Vec2 frontleftpos = b2Vec2(position.x + (-3.0f * CAR_SCALE), position.y + (8.5f * CAR_SCALE));
    tire = new Tire(world, frontleftpos);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.Initialize(m_body, tire->m_body, frontleftpos);
    flJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    m_tires.push_back(tire);
    
    // front right tire
    b2Vec2 frontrightpos = b2Vec2(position.x + (3.0f * CAR_SCALE), position.y + (8.5f * CAR_SCALE));
    tire = new Tire(world, frontrightpos);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.Initialize(m_body, tire->m_body, frontrightpos);
    frJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    m_tires.push_back(tire);
}

Car::~Car()
{
    for (int i = 0; i < m_tires.size(); i++) {
        delete m_tires[i];
    }
}

void Car::update(int controlState)
{
    for (int i = 0; i < m_tires.size(); i++)
        m_tires[i]->updateFriction();
    for (int i = 0; i < m_tires.size(); i++)
        m_tires[i]->updateDrive(controlState);

    //control steering
//    float lockAngle = CC_DEGREES_TO_RADIANS(35);
    float lockAngle = CC_DEGREES_TO_RADIANS(20);
    float turnSpeedPerSec = CC_DEGREES_TO_RADIANS(160);//from lock to lock in 0.5 sec
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    switch ( controlState & (CTRL_LEFT|CTRL_RIGHT) ) {
        case CTRL_LEFT:  desiredAngle = lockAngle;  break;
        case CTRL_RIGHT: desiredAngle = -lockAngle; break;
        default: ;//nothing
    }
    float angleNow = flJoint->GetJointAngle();
    float angleToTurn = desiredAngle - angleNow;
    angleToTurn = b2Clamp( angleToTurn, -turnPerTimeStep, turnPerTimeStep );
    float newAngle = angleNow + angleToTurn;
    flJoint->SetLimits( newAngle, newAngle );
    frJoint->SetLimits( newAngle, newAngle );
//    float rad = CC_DEGREES_TO_RADIANS(m_tires[2]->m_body->GetAngle() + 90);
//    log("angle: %f", m_tires[2]->m_body->GetAngle() + 90);
//    log("rad: %f", rad);
//    log("cos: %f", cosf(rad));
//    log("sin: %f", sinf(rad));
}