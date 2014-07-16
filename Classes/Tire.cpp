//
//  Tire.cpp
//  b2dcar
//
//  Created by 野口一也 on 2014/07/09.
//
//

#include "Tire.h"
#include "InterfaceLayer.h"

Tire::Tire(b2World* world, b2Vec2 position)
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(position.x, position.y);
    m_body = world->CreateBody(&bodydef);
    
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(  0.5f * CAR_SCALE, 1.25f * CAR_SCALE);
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 1); // shape, density
    
    m_body->SetUserData( this );
    
    m_currentTraction = 1;
}

Tire::~Tire()
{
    m_body->GetWorld()->DestroyBody(m_body);
}

void Tire::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse)
{
    m_maxForwardSpeed   = maxForwardSpeed;
    m_maxBackwardSpeed  = maxBackwardSpeed;
    m_maxDriveForce     = maxDriveForce;
    m_maxLateralImpulse = maxLateralImpulse;
}

b2Vec2 Tire::getLateralVelocity()
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(1,0) );
    return b2Dot( currentRightNormal, m_body->GetLinearVelocity() ) * currentRightNormal;
}

b2Vec2 Tire::getForwardVelocity()
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
    return b2Dot( currentForwardNormal, m_body->GetLinearVelocity() ) * currentForwardNormal;
}

void Tire::updateFriction()
{
    //lateral linear velocity
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    if ( impulse.Length() > m_maxLateralImpulse )
        impulse *= m_maxLateralImpulse / impulse.Length();
    m_body->ApplyLinearImpulse( m_currentTraction * impulse, m_body->GetWorldCenter(), true );
    
    //angular velocity
    m_body->ApplyAngularImpulse( m_currentTraction * 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true );
    
    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    m_body->ApplyForce( m_currentTraction * dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true );
}

void Tire::updateDrive(int controlState)
{
    //find desired speed
    float desiredSpeed = 0;
    switch ( controlState & (CTRL_ACCEL|CTRL_BACK) ) {
        case CTRL_ACCEL:   desiredSpeed = m_maxForwardSpeed;  break;
        case CTRL_BACK: desiredSpeed = m_maxBackwardSpeed; break;
        default: return;//do nothing
    }
    
    //find current speed in forward direction
    b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = b2Dot( getForwardVelocity(), currentForwardNormal );
    
    //apply necessary force
    float force = 0;
    if ( desiredSpeed > currentSpeed ) {
        force = m_maxDriveForce;
    } else if ( desiredSpeed < currentSpeed ) {
        force = -m_maxDriveForce;
    } else {
        return;
    }
    m_body->ApplyForce( m_currentTraction * force * currentForwardNormal, m_body->GetWorldCenter(), true );
}